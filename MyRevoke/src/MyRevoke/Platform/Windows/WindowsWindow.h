#pragma once


#include"MyRevoke/Window.h"

#include<GLFW/glfw3.h>

namespace Revoke
{
	class WindowsWindow : public Window
	{
	public:
		WindowsWindow(const WindowSettings& settings);
		virtual ~WindowsWindow();

		void OnUpdate() override;

		inline unsigned int GetWidth() const override { return m_Data.Width; }
		inline unsigned int GetHeight() const override { return m_Data.Height; }

		inline void SetEventCallback(const std::function<void(Event&)>& callback) override { m_Data.EventCallback = callback; }
		void SetVSync(bool enable) override;
		bool IsVSync() const override;

		inline void* GetCoreWindow() const override { return m_Window; }
	private:
		virtual void Init(const WindowSettings& settings);
		virtual void Shutdown();
	private:
		GLFWwindow* m_Window;

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