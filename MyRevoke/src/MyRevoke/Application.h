#pragma once

#include "Core.h"

#include "Window.h"
#include "EventSystem/Event.h"
#include "EventSystem/AppEvent.h"
#include "EventSystem/MouseEvent.h"
#include "EventSystem/KeyEvent.h"
#include "LayerStack.h"
#include "MyRevoke/ImGui/ImGuiLayer.h"
#include "MyRevoke/Renderer/Shader.h"
#include "MyRevoke/Renderer/Buffer.h"
#include "MyRevoke/Renderer/Cmaera.h"

#include "MyRevoke/Core/Time.h"

namespace Revoke
{
	class  Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();

		void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);

		inline static Application& Get() { return *s_Instance; }
		inline Window& GetWindow() { return *m_Window; }

	private:
		bool OnWindowsClose(WindowsCloseEvent e);
	private:
		std::unique_ptr<Window> m_Window;

		LayerStack m_LayerStack;
		ImGuiLayer* m_ImGuiLayer;

		float m_LastFrame;

		bool m_Run = true;

	private:
		static Application* s_Instance;
	};

	//This function will be defined by a client
	Application* CreateApplication();
}
