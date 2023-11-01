#pragma once

#include "Core.h"

#include "Window.h"
#include "EventSystem/Event.h"
#include "EventSystem/AppEvent.h"
#include "EventSystem/MouseEvent.h"
#include "EventSystem/KeyEvent.h"
#include "LayerStack.h"


namespace Revoke
{
	class RV_API Application
	{
	public:
		Application();
		virtual ~Application() {}

		void Run();

		void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);

		inline static Application& Get() { return *s_Instance; }
		inline Window& GetWindow() { return *m_Window; }

	private:
		bool OnWindowsClose(WindowsCloseEvent e);

		std::unique_ptr<Window> m_Window;
		bool m_Run = true;
		LayerStack m_LayerStack;
	private:
		static Application* s_Instance;
	};

	//This function will be defined by a client
	Application* CreateApplication();
}
