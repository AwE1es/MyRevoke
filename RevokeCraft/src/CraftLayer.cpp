#include "CraftLayer.h"

#include <chrono>
#include <string>

#include "MyRevoke/Platform/Windows/FileExplorer.h"
#include "MyRevoke/Math/Math.h"

#include <ImGuizmo.h>

namespace Revoke
{
	extern const std::filesystem::path g_AssetsDirectory = "assets";

	CraftLayer::CraftLayer()
		:Layer("CraftLayer")
	{

	}
	void CraftLayer::OnAttach()
	{
		
		FrameBufferStats frameBufferStats;
		frameBufferStats.Attachments = { FramebufferTextureFormat::RGBA8,FramebufferTextureFormat::RED_INTEGER, FramebufferTextureFormat::DEPTH24STENCIL8 };
		frameBufferStats.Width = 1280;
		frameBufferStats.Height = 720;
		m_FrameBuffer = FrameBuffers::Create(frameBufferStats);
		m_Scene = std::make_shared<Scene>("Main scene");

		m_EditorCamera = EditorCamera(30.0f, 1.778f, 0.1f, 1000.0f);

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
			m_EditorCamera.SetViewportSize(m_ViewportSize.x, m_ViewportSize.y);

			m_Scene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
		}

	
		Renderer2D::ResetStatistics();

		m_FrameBuffer->Bind();
		RenderCommand::ClearColor({ 0.2f, 0.2f, 0.2f, 1.0f });
		RenderCommand::Clear();
		RenderCommand::EnableBlending();

		m_FrameBuffer->ClearColorTextureAttachment(1, -1);

		//TODO: Fix the picking in a play mode!!!
		switch (m_ToolBar.GetSceneState())
		{
		case SceneState::Editor:
		{
			if (m_ViewportFocused)
			{
				m_EditorCamera.OnUpdate(deltaTime);
			}

			m_Scene->OnEditorUpdate(deltaTime, m_EditorCamera);
			break;
		}
		case SceneState::Runtime:
		{
			m_Scene->OnRuntimeUpdate(deltaTime);
			break;
		}
		}

		auto [mx, my] = ImGui::GetMousePos();
		mx -= m_ViewportBounds[0].x;
		my -= m_ViewportBounds[0].y;
		glm::vec2 viewportSize = m_ViewportBounds[1] - m_ViewportBounds[0];
		my = viewportSize.y - my;
		int mouseX = (int)mx;
		int mouseY = (int)my;
		if (mouseX >= 0 && mouseY >= 0 && mouseX < (int)viewportSize.x && mouseY < (int)viewportSize.y)
		{
			int pixelData = m_FrameBuffer->ReadPixel(1, mouseX, mouseY);
			m_HoveredEntity = pixelData == -1 ? Entity() : Entity((entt::entity)pixelData, m_Scene.get());
		}
		
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
				if (ImGui::MenuItem("New Scene", "Ctrl+N")) NewScene();
				if (ImGui::MenuItem("Open", "Ctrl+O")) OpenScene();
				if (ImGui::MenuItem("Save", "Ctrl+S")) Save();
				if (ImGui::MenuItem("Save as", "Ctrl+Shift+S")) SaveAs();
	
				ImGui::EndMenu();
			}
			

			ImGui::EndMenuBar();
		}

		//-----------Settings-------------------------------------------------
	
		//--------------------------------------------------------------------
		m_ObjPannel.OnImGuiRender();
		m_ContentBrowserPanel.OnImGuiRender();
		//-----------ViewPort-------------------------------------------------
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
		ImGui::Begin("Viewport");

		auto viewportMinRegion = ImGui::GetWindowContentRegionMin();
		auto viewportMaxRegion = ImGui::GetWindowContentRegionMax();
		auto viewportOffset = ImGui::GetWindowPos();
		m_ViewportBounds[0] = { viewportMinRegion.x + viewportOffset.x, viewportMinRegion.y + viewportOffset.y };
		m_ViewportBounds[1] = { viewportMaxRegion.x + viewportOffset.x, viewportMaxRegion.y + viewportOffset.y };

		m_ViewportFocused = ImGui::IsWindowFocused();
		m_ViewportHovered = ImGui::IsWindowHovered();
		Application::Get().GetImGuiLayer()->BlockEvents(!m_ViewportFocused && !m_ViewportHovered);

		ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
		m_ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };
		uint32_t textureID = m_FrameBuffer->GetColorAttachmentRendererID();
	
		Entity selectedEntity = m_ObjPannel.GetSelectedEntity();

		ImGui::Image((void*)textureID, ImVec2{ m_ViewportSize.x, m_ViewportSize.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });

		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_PAYLOAD"))
			{
				const wchar_t* path = (const wchar_t*)payload->Data;
				OpenScene(std::filesystem::path(g_AssetsDirectory) / path);
			}

			ImGui::EndDragDropTarget();
		}
		

		//-----------Guizmo---------------------------------------------------
		if (selectedEntity && m_GizmoType != -1)
		{
			ImGuizmo::SetOrthographic(false);
			ImGuizmo::SetDrawlist();

			ImGuizmo::SetRect(m_ViewportBounds[0].x, m_ViewportBounds[0].y, m_ViewportBounds[1].x - m_ViewportBounds[0].x, m_ViewportBounds[1].y - m_ViewportBounds[0].y);


			auto mainCameraEntt = m_EditorCamera;
			const glm::mat4& cameraProjection = m_EditorCamera.GetProjectionMatrix();
			glm::mat4 cameraView = m_EditorCamera.GetViewMatrix();

			auto& entityTransformsComponent = selectedEntity.GetComponent<TransformComponent>();
			glm::mat4 tranforms = entityTransformsComponent.GetTransform();
			
			bool snap = Input::IsKeyPressed(RV_KEY_LEFT_CONTROL);
			float snapValue = 0.5f;
			if (m_GizmoType == ImGuizmo::OPERATION::ROTATE)
				snapValue = 45.0f;

			float snapValues[3] = { snapValue, snapValue, snapValue };

		
			ImGuizmo::Manipulate(glm::value_ptr(cameraView), glm::value_ptr(cameraProjection), (ImGuizmo::OPERATION)m_GizmoType, ImGuizmo::LOCAL, glm::value_ptr(tranforms), nullptr, snap ? snapValues : nullptr);

			if (ImGuizmo::IsUsing() && !Input::IsKeyPressed(RV_KEY_LEFT_ALT))
			{
				glm::vec3 translation, rotation, scale;
				DecomposeTransform(tranforms, translation, rotation, scale);

				glm::vec3 deltaRotation = rotation - entityTransformsComponent.Rotation;
				entityTransformsComponent.Position = translation;
				entityTransformsComponent.Rotation += deltaRotation;
				entityTransformsComponent.Scale = scale;
			}
		}


		m_ToolBar.OnImGuiRender();

		ImGui::End();
		ImGui::PopStyleVar();

		ImGui::End();
	}

	void CraftLayer::OnEvent(Revoke::Event& e)
	{
		m_EditorCamera.OnEvent(e);

		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<KeyPressedEvent>(RV_BIND_EVENT_FUNK(CraftLayer::OnKeyPressed));
		dispatcher.Dispatch<MouseButtonPressedEvent>(RV_BIND_EVENT_FUNK(CraftLayer::OnMouseBtnPressed));
	}
	bool CraftLayer::OnMouseBtnPressed(MouseButtonPressedEvent& e)
	{
		if (e.GetMouseButton() == RV_MOUSE_BUTTON_1 && !Input::IsKeyPressed(RV_KEY_LEFT_ALT))
		{
			if (m_ViewportHovered && !ImGuizmo::IsOver())
			{
				m_ObjPannel.SetSelectedEntity(m_HoveredEntity);
			}
			return true;
		}
		return false;
	}
	bool CraftLayer::OnKeyPressed(KeyPressedEvent& e)
	{
		switch (e.GetKeyCode())
		{
		case RV_KEY_S:
		{
			if (Input::IsKeyPressed(RV_KEY_LEFT_CONTROL) && Input::IsKeyPressed(RV_KEY_LEFT_SHIFT))
			{
				SaveAs();
			}
			else if (Input::IsKeyPressed(RV_KEY_LEFT_CONTROL))
			{
				Save();
			}
			break;
		}
		case RV_KEY_N:
		{
			if (Input::IsKeyPressed(RV_KEY_LEFT_CONTROL))
			{
				NewScene();
			}
			break;
		}
		case RV_KEY_O:
		{
			if (Input::IsKeyPressed(RV_KEY_LEFT_CONTROL))
			{
				OpenScene();
			}
			break;
		}
		case RV_KEY_Q:
		{
			m_GizmoType = -1;
			break;
		}
		case RV_KEY_W:
		{
			m_GizmoType = ImGuizmo::OPERATION::TRANSLATE;
			break;
		}
		case RV_KEY_E:
		{
			m_GizmoType = ImGuizmo::OPERATION::ROTATE;
			break;
		}
		case RV_KEY_R:
		{
			m_GizmoType = ImGuizmo::OPERATION::SCALE;
			break;
		}
		}
		return false;
	}

	//------------------------------------------------------------------------
	void CraftLayer::NewScene()
	{
		m_Scene = std::make_shared<Scene>("New scene");
		m_Scene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
		m_ObjPannel.SetScene(m_Scene);
	}

	void CraftLayer::OpenScene()
	{
		std::string path = FileExplorer::OpenFile("MyRevoke Scene (*.myrevoke)\0*.myrevoke\0");

		if (!path.empty())
		{
			OpenScene(path);
		}
	}
	void CraftLayer::OpenScene(const std::filesystem::path& path)
	{

		m_Scene = std::make_shared<Scene>();
		m_Scene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
		m_ObjPannel.SetScene(m_Scene);

		Serealizer sceneSerealizer(m_Scene);
		sceneSerealizer.DeSerealize(path.string());
		
	}

	void CraftLayer::SaveAs()
	{
		std::string path = FileExplorer::SaveFile("Hazel Scene (*.myrevoke)\0*.myrevoke\0");

		if (!path.empty())
		{
			Serealizer sceneSerealizer(m_Scene);
			sceneSerealizer.Serealize(path+".myrevoke");
		}
	}

	void CraftLayer::Save()
	{
		Serealizer sceneSerealizer(m_Scene);
		sceneSerealizer.Serealize("Scenes/" + m_Scene->GetName() + ".myrevoke");
	}

}

	
