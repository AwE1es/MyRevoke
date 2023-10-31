#include "rvpch.h"
#include "Application.h"
#include "Log.h"

#include <GLFW/glfw3.h>

namespace Revoke
{
#define BIND_EVENT_FUNC(x) std::bind(&Application::x, this, std::placeholders::_1)
	Application::Application()
	{
		m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window->SetEventCallback(std::bind(&Application::OnEvent, this, std::placeholders::_1));
	}
	void Application::Run()
	{
	
		while (m_Run)
		{
			glClearColor(1, 0, 1, 1);
			glClear(GL_COLOR_BUFFER_BIT);
			m_Window->OnUpdate();
		}
	}
	void Application::OnEvent(Event& e)
	{

		EventDispatcher eDispatcher(e);
		
		eDispatcher.Dispatch<WindowsCloseEvent>(BIND_EVENT_FUNC(OnWindowsClose));

		RV_CORE_TRACE("{0}", e);
	}
	bool Application::OnWindowsClose(WindowsCloseEvent e)
	{
		m_Run = false;
		return true;
	}
}