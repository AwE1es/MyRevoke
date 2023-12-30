#include "CraftLayer.h"

#include <chrono>
#include <string>

namespace Revoke
{
	template<typename Fn>
	class Timer
	{
	public:
		Timer(const char* name, Fn&& func)
			: m_Name(name), m_Func(func), m_Stopped(false)
		{
			m_StartTimepoint = std::chrono::high_resolution_clock::now();
		}

		~Timer()
		{
			if (!m_Stopped)
				Stop();
		}

		void Stop()
		{
			auto endTimepoint = std::chrono::high_resolution_clock::now();

			long long start = std::chrono::time_point_cast<std::chrono::microseconds>(m_StartTimepoint).time_since_epoch().count();
			long long end = std::chrono::time_point_cast<std::chrono::microseconds>(endTimepoint).time_since_epoch().count();

			m_Stopped = true;

			float duration = (end - start) * 0.001f;
			m_Func({ m_Name, duration });
		}
	private:
		const char* m_Name;
		Fn m_Func;
		std::chrono::time_point<std::chrono::steady_clock> m_StartTimepoint;
		bool m_Stopped;
	};

#define PROFILE_SCOPE(name) Timer timer##__LINE__(name, [&](ProfileResult profileResult) { m_ProfileResults.push_back(profileResult); })


	CraftLayer::CraftLayer()
		:Layer("Testing")
	{

	}
	void CraftLayer::OnAttach()
	{


		m_Texture2D = Texture2D::Create("Textures/Triangle_Texture.png");
		
		FrameBufferStats frameBufferStats;
		frameBufferStats.Width = 1280;
		frameBufferStats.Height = 720;
		m_FrameBuffer = FrameBuffers::Create(frameBufferStats);
		m_Scene = std::make_shared<Scene>();

		//Entity Creation
		auto square = m_Scene->CreateEntity("Square1");
		square.AddComponent<SpriteRendererComponent>(glm::vec4(0.0f, 0.2f, 0.5f, 1.0f));
		
		m_SquereEntity = square;

		m_CameraEntity = m_Scene->CreateEntity("Camera Entity");
		m_CameraEntity.AddComponent<CameraComponent>();

		m_SecondCamera = m_Scene->CreateEntity("Clip-Space Entity");
		auto& cc = m_SecondCamera.AddComponent<CameraComponent>();
		cc.isMain = false;

		class CameraController : public ScriptEntity
		{
		public:
			void OnCreate()
			{
				auto& transform = GetComponent<TransformComponent>().Transform;
				transform[3][0] = rand() % 10 - 5.0f;
			}

			void OnDestroy()
			{
			}

			void OnUpdate(Timestep ts)
			{
				auto& transform = GetComponent<TransformComponent>().Transform;
				float speed = 5.0f;

				if (Input::IsKeyPressed(RV_KEY_A))
					transform[3][0] -= speed * ts;
				if (Input::IsKeyPressed(RV_KEY_D))
					transform[3][0] += speed * ts;
				if (Input::IsKeyPressed(RV_KEY_W))
					transform[3][1] += speed * ts;
				if (Input::IsKeyPressed(RV_KEY_S))
					transform[3][1] -= speed * ts;
			}
		};

		m_CameraEntity.AddComponent<NativeScriptComponent>().Bind<CameraController>();

		m_SecondCamera.AddComponent<NativeScriptComponent>().Bind<CameraController>();

	}
	void CraftLayer::OnDetach()
	{

	}
	void CraftLayer::OnUpdate(Timestep deltaTime)
	{
		PROFILE_SCOPE("Sandbox2D::OnUpdate");


		// Resize
		if (FrameBufferStats spec = m_FrameBuffer->GetSpecification();
			m_ViewportSize.x > 0.0f && m_ViewportSize.y > 0.0f && 
			(spec.Width != m_ViewportSize.x || spec.Height != m_ViewportSize.y))
		{
			m_FrameBuffer->Resize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);

			m_Scene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
		}


		if (m_ViewportFocused)
			//m_CameraController2D.OnUpdate(deltaTime);

		Renderer2D::ResetStatistics();

		m_FrameBuffer->Bind();
		RenderCommand::ClearColor({ 0.2f, 0.2f, 0.2f, 1.0f });
		RenderCommand::Clear();
		RenderCommand::EnableBlending();

		m_Scene->OnUpdate(deltaTime);

		m_FrameBuffer->UnBind();

	}

	void CraftLayer::OnImGuiDraw()
	{
		static bool dockspaceOpen = true;
		static bool opt_fullscreen_persistant = true;
		bool opt_fullscreen = opt_fullscreen_persistant;
		static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

		// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
		// because it would be confusing to have two docking targets within each others.
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
		if (opt_fullscreen)
		{
			ImGuiViewport* viewport = ImGui::GetMainViewport();
			ImGui::SetNextWindowPos(viewport->Pos);
			ImGui::SetNextWindowSize(viewport->Size);
			ImGui::SetNextWindowViewport(viewport->ID);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
			window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
			window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
		}

		// When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background and handle the pass-thru hole, so we ask Begin() to not render a background.
		if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
			window_flags |= ImGuiWindowFlags_NoBackground;

		// Important: note that we proceed even if Begin() returns false (aka window is collapsed).
		// This is because we want to keep our DockSpace() active. If a DockSpace() is inactive, 
		// all active windows docked into it will lose their parent and become undocked.
		// We cannot preserve the docking relationship between an active window and an inactive docking, otherwise 
		// any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("DockSpace Demo", &dockspaceOpen, window_flags);
		ImGui::PopStyleVar();

		if (opt_fullscreen)
			ImGui::PopStyleVar(2);

		// DockSpace
		ImGuiIO& io = ImGui::GetIO();
		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
		{
			ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
		}

		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				// Disabling fullscreen would allow the window to be moved to the front of other windows, 
				// which we can't undo at the moment without finer window depth/z control.
				//ImGui::MenuItem("Fullscreen", NULL, &opt_fullscreen_persistant);

				if (ImGui::MenuItem("Exit")) Application::Get().Close();
				ImGui::EndMenu();
			}

			ImGui::EndMenuBar();
		}

		//-----------Settings-------------------------------------------------
		ImGui::Begin("Settings");
		if (m_SquereEntity)
		{
			ImGui::Separator();
			auto& tag = m_SquereEntity.GetComponent<NameCpmponent>().Name;
			ImGui::Text("%s", tag.c_str());

			auto& squareColor = m_SquereEntity.GetComponent<SpriteRendererComponent>().Color;
			ImGui::ColorEdit4("Square Color", glm::value_ptr(squareColor));
			ImGui::Separator();
		}
		ImGui::Separator();

		ImGui::DragFloat3("Camera Transform",
			glm::value_ptr(m_CameraEntity.GetComponent<TransformComponent>().Transform[3]));

		if (ImGui::Checkbox("Camera A", &m_PrimaryCamera))
		{
			m_CameraEntity.GetComponent<CameraComponent>().isMain = m_PrimaryCamera;
			m_SecondCamera.GetComponent<CameraComponent>().isMain = !m_PrimaryCamera;
		}

		auto& camera = m_SecondCamera.GetComponent<CameraComponent>().Camera;
		float orthoSize = camera.GetOrthographicSize();
		if (ImGui::DragFloat("Second Camera Ortho Size", &orthoSize))
			camera.SetOrthographicSize(orthoSize);

		ImGui::Separator();
		auto stats = Revoke::Renderer2D::GetStats();
		ImGui::Text("Renderer Stats");
		ImGui::Text("Draw Calls: %d", stats.DrawCalls);
		ImGui::Text("Quad Count: %d", stats.QuadCount);
		ImGui::Text("Index Count: %d", stats.GetTotalIndexCount());
		ImGui::Text("Vertex Count: %d", stats.GetTotalVertexCount());
		ImGui::Text("---------------------");
		float totalFrameTime = 0.0f;
		float displayUpdateInterval = 0.25f;

		for (auto& result : m_ProfileResults)
		{
			totalFrameTime += result.Time;
			char label[50];
			sprintf(label, "%.3fms %s", result.Time, result.Name);
			ImGui::Text(label);
		}

		float averageFrameTime = totalFrameTime / m_ProfileResults.size();
		float frameRate = 1000.0f / averageFrameTime;

		static float timeSinceLastUpdate = 0.0f;
		timeSinceLastUpdate += ImGui::GetIO().DeltaTime;

		if (timeSinceLastUpdate >= displayUpdateInterval)
		{
			m_Framerate = frameRate;
			timeSinceLastUpdate = 0.0f;
		}
		char frameRateLabel[50];
		sprintf(frameRateLabel, "FPS: %.2f FPS", m_Framerate);
		ImGui::Text(frameRateLabel);

		m_ProfileResults.clear();
		ImGui::End();

		//-----------ViewPort-------------------------------------------------
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
		ImGui::Begin("Viewport");

		m_ViewportFocused = ImGui::IsWindowFocused();
		m_ViewportHovered = ImGui::IsWindowHovered();
		Application::Get().GetImGuiLayer()->BlockEvents(!m_ViewportFocused || !m_ViewportHovered);

		ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
		m_ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };
		uint32_t textureID = m_FrameBuffer->GetColorAttachmentRendererID();
		ImGui::Image((void*)textureID, ImVec2{ m_ViewportSize.x, m_ViewportSize.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
		ImGui::End();
		ImGui::PopStyleVar();

		ImGui::End();
	}

	void CraftLayer::OnEvent(Revoke::Event& e)
	{
		//m_CameraController2D.OnEvent(e);
	}

}

	
