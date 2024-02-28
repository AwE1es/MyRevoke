#include "CraftLayer.h"

#include <chrono>
#include <string>
#include "SceneUi.h"

namespace Revoke
{
	CraftLayer::CraftLayer()
		:Layer("CraftLayer")
	{

	}
	void CraftLayer::OnAttach()
	{
		
		FrameBufferStats frameBufferStats;
		frameBufferStats.Width = 1280;
		frameBufferStats.Height = 720;
		m_FrameBuffer = FrameBuffers::Create(frameBufferStats);
		m_Scene = std::make_shared<Scene>();

		//Entity Creation
		{
			auto square = m_Scene->CreateEntity("Square B");
			square.AddComponent<SpriteRendererComponent>(glm::vec4(0.1f, 0.2f, 0.55f, 1.0f));
		}

		{
			auto square = m_Scene->CreateEntity("Square A");
			square.AddComponent<SpriteRendererComponent>(glm::vec4(0.0f, 0.2f, 0.5f, 1.0f));
		}

		{
			auto square = m_Scene->CreateEntity("Teiangle Texture");
			square.AddComponent<SpriteRendererComponent>("Textures/Triangle_Texture.png");
		}

		m_CameraEntity = m_Scene->CreateEntity("Main Camera");
		m_CameraEntity.AddComponent<CameraComponent>();

		m_SecondCamera = m_Scene->CreateEntity("Second Camera");
		auto& cc = m_SecondCamera.AddComponent<CameraComponent>();
		cc.isMain = false;

		class CameraController : public ScriptEntity
		{
		public:
			void OnCreate()
			{
				auto& transform = GetComponent<TransformComponent>();
				transform.Position.x = rand() % 10 - 5.0f;
			}

			void OnDestroy()
			{
			}

			void OnUpdate(Timestep ts)
			{
				auto& transform = GetComponent<TransformComponent>().Position;
				float speed = 5.0f;

				auto& camera = GetComponent<CameraComponent>().Camera;
				if (camera.GetProjectionType() == SceneCamera::Projection::Orthographic)
				{
					if (Input::IsKeyPressed(RV_KEY_A))
						transform.x -= speed * ts;
					if (Input::IsKeyPressed(RV_KEY_D))
						transform.x += speed * ts;
					if (Input::IsKeyPressed(RV_KEY_W))
						transform.y += speed * ts;
					if (Input::IsKeyPressed(RV_KEY_S))
						transform.y -= speed * ts;
				}
				else
				{
					//RV_TRACE("Mouse x -> ", Input::GetMouseX());
					//RV_TRACE("Mouse x -> ", Input::GetMouseY());
					if (Input::IsKeyPressed(RV_KEY_A))
						transform.x -= speed * ts;
					if (Input::IsKeyPressed(RV_KEY_D))
						transform.x += speed * ts;
					if (Input::IsKeyPressed(RV_KEY_SPACE))
						transform.y += speed * ts;
					if (Input::IsKeyPressed(RV_KEY_LEFT_SHIFT))
						transform.y -= speed * ts;
					if (Input::IsKeyPressed(RV_KEY_W))
						transform.z -= speed * ts;
					if (Input::IsKeyPressed(RV_KEY_S))
						transform.z += speed * ts;
				}
			}
		};

		m_CameraEntity.AddComponent<NativeScriptComponent>().Bind<CameraController>();

		m_SecondCamera.AddComponent<NativeScriptComponent>().Bind<CameraController>();

		m_ObjPannel.SetScene(m_Scene);
	}
	void CraftLayer::OnDetach()
	{

	}
	void CraftLayer::OnUpdate(Timestep deltaTime)
	{
		// Resize
		if (FrameBufferStats spec = m_FrameBuffer->GetSpecification();
			m_ViewportSize.x > 0.0f && m_ViewportSize.y > 0.0f && 
			(spec.Width != m_ViewportSize.x || spec.Height != m_ViewportSize.y))
		{
			m_FrameBuffer->Resize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);

			m_Scene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
		}

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
		if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
			window_flags |= ImGuiWindowFlags_NoBackground;

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
				if (ImGui::MenuItem("Exit")) Application::Get().Close();
				ImGui::EndMenu();
			}

			ImGui::EndMenuBar();
		}

		//-----------Settings-------------------------------------------------
	
		//--------------------------------------------------------------------
		m_ObjPannel.OnImGuiRender();
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
	
	}

}

	
