#include "rvpch.h"
#include "Application.h"
#include "Log.h"

#include <glad/glad.h>

namespace Revoke
{
#define BIND_EVENT_FUNC(x) std::bind(&Application::x, this, std::placeholders::_1)
	
	Application* Application::s_Instance = nullptr;
	
	Application::Application()
	{
		RV_CORE_ASSERT(!s_Instance, "Applicatio already exist");
		s_Instance = this;

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
			for (Layer* layer : m_LayerStack)
				layer->OnUpdate();
		}
	}
	void Application::OnEvent(Event& e)
	{

		EventDispatcher eDispatcher(e);
		
		eDispatcher.Dispatch<WindowsCloseEvent>(BIND_EVENT_FUNC(OnWindowsClose));

		RV_CORE_TRACE("{0}", e);

		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();)
		{
			(*--it)->OnEvent(e);
			if (e.Handled)
				break;
		}

	}
	void Application::PushLayer(Layer* layer)
	{
		m_LayerStack.PushLayer(layer);
		layer->OnAttach();
	}
	void Application::PushOverlay(Layer* layer)
	{
		m_LayerStack.PushOverlay(layer);
		layer->OnAttach();
	}
	bool Application::OnWindowsClose(WindowsCloseEvent e)
	{
		m_Run = false;
		return true;
	}
}