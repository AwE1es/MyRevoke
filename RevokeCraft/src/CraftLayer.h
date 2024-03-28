#include "MyRevoke.h"
#include "SceneUi.h"

#include "imgui.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp" 
#include <glm/gtc/type_ptr.hpp> 

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

		bool OnKeyPressed(KeyPressedEvent& e);

		void NewScene();
		void OpenScene();
		void SaveAs();
		void Save();

	private:

		Shared<FrameBuffers> m_FrameBuffer;

		Shared<Scene> m_Scene;

		Entity m_CameraEntity;
		Entity m_SecondCamera;

		ObjectsPannel m_ObjPannel;

		bool m_PrimaryCamera = true;
		glm::vec2 m_ViewportSize = { 0.0f, 0.0f };
		bool m_ViewportFocused = false;
		bool m_ViewportHovered = false;

		float m_Framerate = 0.0f;

		int m_GizmoType = -1;

		struct ProfileResult
		{
			const char* Name;
			float Time;
		};
	};
}