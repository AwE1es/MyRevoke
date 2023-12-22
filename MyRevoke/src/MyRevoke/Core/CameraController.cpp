#include "rvpch.h"
#include "CameraController.h"

#include "Core.h"


namespace Revoke
{
	OrthoCameraController::OrthoCameraController(float aspectRatio)
		: m_AspectRatio(aspectRatio), m_Camera(-m_AspectRatio * m_ZoomLevel, m_AspectRatio* m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel)
	{

	}

	void OrthoCameraController::OnEvent(Event& e)
	{
		EventDispatcher d(e);
		d.Dispatch<MouseScrolledEvent>(RV_BIND_EVENT_FUNK(OrthoCameraController::OnMouseScroll));
		d.Dispatch<WindowResizeEvent>(RV_BIND_EVENT_FUNK(OrthoCameraController::OnWindowsResize));
	}

	void OrthoCameraController::OnUpdate(const Timestep& ts)
	{
		if (Revoke::Input::IsKeyPressed(RV_KEY_LEFT))
		{
			m_CameraPos.x -= m_CamSpeed * ts.GetSeconds();
		}
		if (Revoke::Input::IsKeyPressed(RV_KEY_RIGHT))
		{
			m_CameraPos.x += m_CamSpeed * ts.GetSeconds();
		}

		if (Revoke::Input::IsKeyPressed(RV_KEY_UP))
		{
			m_CameraPos.y += m_CamSpeed * ts.GetSeconds();
		}
		if (Revoke::Input::IsKeyPressed(RV_KEY_DOWN))
		{
			m_CameraPos.y -= m_CamSpeed * ts.GetSeconds();
		}

		m_Camera.SetCameraPos(m_CameraPos);

	}
	bool OrthoCameraController::OnMouseScroll(MouseScrolledEvent& e)
	{
		m_ZoomLevel -= e.GetDeltaY() * 0.25f;
		m_ZoomLevel = std::max(m_ZoomLevel, 0.25f);
		m_Camera.CalculateProjectionMatrix(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
		return false;
	}
	bool OrthoCameraController::OnWindowsResize(WindowResizeEvent& e)
	{
		OnResize((float)e.GetWidth(), (float)e.GetHeight());
		return false;
	}
	void OrthoCameraController::OnResize(float width, float height)
	{
		m_AspectRatio = width / height;
		m_Camera.SetFov(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
	}

};
