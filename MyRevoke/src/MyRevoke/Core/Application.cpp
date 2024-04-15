#include "rvpch.h"
#include "Application.h"
#include "Log.h"
#include "Input.h"

#include "MyRevoke/Renderer/Renderer2D.h"

#include "GLFW/glfw3.h"

namespace Revoke
{
	
	Application* Application::s_Instance = nullptr;
	

	Application::Application()
	
	{
		RV_CORE_ASSERT(!s_Instance, "Applicatio already exist");
		s_Instance = this;

		m_Window = std::make_shared<Window>();
		m_Window->SetEventCallback(RV_BIND_EVENT_FUNK(Application::OnEvent));


		RendererAPI::Init();
		Renderer2D::Init();

		m_ImGuiLayer = new ImGuiLayer();
		PushOverlay(m_ImGuiLayer);
	

	}
	Application::~Application()
	{
		Renderer2D::Shutdown();
	}
	void Application::Run()
	{
	
		while (m_Run)
		{
			float time = (float)glfwGetTime();
			Timestep timestep = time - m_LastFrame;
			m_LastFrame = time;

			for (Layer* layer : m_LayerStack)
				layer->OnUpdate(timestep);

			m_ImGuiLayer->Begin();
			for (Layer* layer : m_LayerStack)
				layer->OnImGuiDraw();
			m_ImGuiLayer->End();


			m_Window->OnUpdate();
		}
	}
	void Application::Close()
	{
		m_Run = false;
	}
	void Application::OnEvent(Event& e)
	{
		
		EventDispatcher eDispatcher(e);
		
		eDispatcher.Dispatch<WindowsCloseEvent>(RV_BIND_EVENT_FUNK(Application::OnWindowClose));
		eDispatcher.Dispatch<WindowResizeEvent>(RV_BIND_EVENT_FUNK(Application::OnWindowResize));

		for (auto it = m_LayerStack.rbegin(); it != m_LayerStack.rend();++it)
		{
			if (e.Handled)
				break;
			(*it)->OnEvent(e);
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
	bool Application::OnWindowClose(WindowsCloseEvent e)
	{
		m_Run = false;
		return true;
	}
	bool Application::OnWindowResize(WindowResizeEvent e)
	{
		RendererAPI::WindowResize((float)e.GetWidth(), (float)e.GetHeight());
		return true;
	}
}