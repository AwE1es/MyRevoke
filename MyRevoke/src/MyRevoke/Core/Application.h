#pragma once

#include "Core.h"
#include "Time.h"

#include "Window.h"
#include "LayerStack.h"

#include "MyRevoke/EventSystem/Event.h"
#include "MyRevoke/EventSystem/AppEvent.h"
#include "MyRevoke/EventSystem/MouseEvent.h"
#include "MyRevoke/EventSystem/KeyEvent.h"

#include "MyRevoke/ImGui/ImGuiLayer.h"
#include "MyRevoke/Renderer/Shader.h"
#include "MyRevoke/Renderer/Buffer.h"
#include "MyRevoke/Renderer/Cmaera.h"


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

		void Close();

		ImGuiLayer* GetImGuiLayer() { return m_ImGuiLayer; }

		inline static Application& Get() { return *s_Instance; }
		inline Window& GetWindow() { return *m_Window; }

	private:
		bool OnWindowsClose(WindowsCloseEvent e);
		bool OnWindowResize(WindowResizeEvent e);
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
