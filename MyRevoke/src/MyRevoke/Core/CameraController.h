#pragma once
#include "MyRevoke/Renderer/Cmaera.h"

#include "MyRevoke/EventSystem/Event.h"
#include "MyRevoke/EventSystem/AppEvent.h"
#include "MyRevoke/EventSystem/MouseEvent.h"

#include "Time.h"
#include "Input.h"
#include "KeyCodes.h"

#include "glm/glm.hpp"

namespace Revoke {

	class OrthoCameraController
	{
	public:
		OrthoCameraController(float aspectRatio);

		void OnEvent(Event& e);
		void OnUpdate(const Timestep& ts);

		Camera& GetCamera() { return m_Camera; }
	private:
		bool OnMouseScroll(MouseScrolledEvent& e);
		bool OnWindowsResize(WindowResizeEvent& e);
	private:
		float m_AspectRatio;
		float m_ZoomLevel = 1.0f;

		Camera m_Camera;
		glm::vec3 m_CameraPos = { 0.0f, 0.0f, 0.0f };
		float m_CamSpeed = 3.0f;
	};
}