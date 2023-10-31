#include "rvpch.h"

#include "WindowsWindow.h"


namespace Revoke
{
	static bool s_GLFWInitialization = false;

	

	WindowsWindow::WindowsWindow(const WindowSettings& settings)
	{
		Init(settings);
	}

	Window* Window::Create(const WindowSettings& settings)
	{
		return new WindowsWindow(settings);
	}

	WindowsWindow::~WindowsWindow()
	{
		Shutdown();
	}

	void WindowsWindow::Init(const WindowSettings& settings)
	{
		m_Data.Title = settings.Title;
		m_Data.Width = settings.Width;
		m_Data.Height = settings.Height;
		
		RV_CORE_INFO("Window is initialized {0} ({1}, {2})", settings.Title, settings.Width, settings.Height);

		if (!s_GLFWInitialization)
		{
			//Add glfwTerminate on system shutdown
			int success = glfwInit();
			RV_CORE_ASSERT(success, "Could not initialize GLFW");

			s_GLFWInitialization = true;
		}

		m_Window = glfwCreateWindow((int)settings.Width, (int)settings.Height, m_Data.Title.c_str(), nullptr, nullptr);

		glfwMakeContextCurrent(m_Window);
		glfwSetWindowUserPointer(m_Window, &m_Data);
		SetVSync(true);

	}

	void WindowsWindow::Shutdown()
	{
		glfwDestroyWindow(m_Window);
	}

	void WindowsWindow::OnUpdate()
	{
		glfwPollEvents();
		glfwSwapBuffers(m_Window);
	}

	void WindowsWindow::SetVSync(bool cond)
	{
		if (cond)
			glfwSwapInterval(1);
		else
			glfwSwapInterval(0);
		m_Data.VSync = cond;
	}

	bool WindowsWindow::IsVSync() const
	{
		return m_Data.VSync;
	}

}
