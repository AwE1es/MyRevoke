#pragma once

#include "rvpch.h"

#include "Core.h"
#include "MyRevoke/EventSystem/Event.h"
#include "MyRevoke/Renderer/GraphicContex.h"


namespace Revoke {

	struct WindowSettings
	{
		std::string Title;
		unsigned int Width;
		unsigned int Height;

		WindowSettings(const std::string& title = "MyRevoke",
			unsigned int width = 1280,
			unsigned int height = 720)
			: Title(title), Width(width), Height(height){}
		                               
	};

	class Window
	{
	public:
		Window(const WindowSettings& settings = WindowSettings());

		~Window();

		void OnUpdate();

		unsigned int GetWidth() const { return m_Data.Width; }
		unsigned int GetHeight() const { return m_Data.Height; }

		inline void SetEventCallback(const std::function<void(Event&)>& callback) { m_Data.EventCallback = callback; }
		void SetVSync(bool enable);
		bool IsVSync() const;

		inline void* GetCoreWindow() const { return m_Window; }
	private:
		void Init(const WindowSettings& settings);
		void Shutdown();
	private:
		GLFWwindow* m_Window;
		RenderContex* m_Context;

		struct WindowData
		{
			std::string Title = "";
			unsigned int Width = 0, Height = 0;
			bool VSync = false;
			std::function<void(Event&)> EventCallback;
		};
		WindowData m_Data;

	};
}

