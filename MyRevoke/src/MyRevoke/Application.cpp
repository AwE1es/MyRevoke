#include "rvpch.h"
#include "Application.h"

#include "EventSystem/AppEvent.h"
#include "EventSystem/MouseEvent.h"
#include "Log.h"

#include <GLFW/glfw3.h>

namespace Revoke
{
	Application::Application()
	{
		m_Window = std::unique_ptr<Window>(Window::Create());
	}
	void Application::Run()
	{
	
		while (m_Run)
		{
			m_Window->OnUpdate();
		}
	}
}