#pragma once

#include "Core.h"
#include "EventSystem/Event.h"
#include "Window.h"


namespace Revoke
{
	class RV_API Application
	{
	public:
		Application();
		virtual ~Application() {}

		void Run();

	private:
		std::unique_ptr<Window> m_Window;
		bool m_Run = true;
	};

	//This function will be defined by a client
	Application* CreateApplication();
}
