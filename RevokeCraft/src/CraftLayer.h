#include "MyRevoke.h"

#include <imgui.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp> 

#include <filesystem>

#include "ObjectsPannel.h"
#include "ContentBrowser.h"
#include "ToolBar.h"
#include "SceneSettingsPannel.h"

namespace Revoke
{

	class CraftLayer : public Revoke::Layer
	{
	public:
		CraftLayer();

		void OnAttach() override;
		void OnDetach() override;

		void OnUpdate(Timestep deltaTime) override;
		void OnImGuiDraw() override;
		void OnEvent(Event& e) override;

		bool OnMouseBtnPressed(MouseButtonPressedEvent& e);
		bool OnKeyPressed(KeyPressedEvent& e);

		void NewScene();
		void OpenScene();
		void OpenScene(const std::filesystem::path& path);
		void SaveAs();
		void Save();

		Shared<Scene> GetCurrentScene() const { return m_Scene; }

	private:

		Shared<FrameBuffers> m_FrameBuffer;

		Shared<Scene> m_Scene;

		Entity m_CameraEntity;
		Entity m_SecondCamera;

		Entity m_HoveredEntity;

		EditorCamera m_EditorCamera;

		ObjectsPannel m_ObjPannel;
		ContentBrowser m_ContentBrowserPanel;
		ToolBar m_ToolBar;
		SceneSettingsPannel m_ProjectSettingsPanel;

		bool m_PrimaryCamera = true;
		glm::vec2 m_ViewportSize = { 0.0f, 0.0f };
		bool m_ViewportFocused = false;
		bool m_ViewportHovered = false;

		glm::vec2 m_ViewportBounds[2];

		int* m_GizmoType;

		struct ProfileResult
		{
			const char* Name;
			float Time;
		};

	};
}