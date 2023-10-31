#pragma once

#include "Core.h"
#include "EventSystem/Event.h"
#include "EventSystem/AppEvent.h"
#include "EventSystem/MouseEvent.h"
#include "EventSystem/KeyEvent.h"
#include "Window.h"


namespace Revoke
{
	class RV_API Application
	{
	public:
		Application();
		virtual ~Application() {}

		void Run();

		void OnEvent(Event& e);

		bool OnWindowsClose(WindowsCloseEvent e);

	private:
		std::unique_ptr<Window> m_Window;
		bool m_Run = true;
	};

	//This function will be defined by a client
	Application* CreateApplication();
}
