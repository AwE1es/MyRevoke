#include "rvpch.h"

#include "GraphicContex.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

namespace Revoke
{

	RenderContex::RenderContex(GLFWwindow* windowHandle)
		: m_WindowHandle(windowHandle)
	{
		RV_CORE_ASSERT(windowHandle, "Window handle is null :(");
	}

	void Revoke::RenderContex::Init()
	{
		glfwMakeContextCurrent(m_WindowHandle);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		RV_CORE_ASSERT(status, "Failed GLAD");
	}

	void Revoke::RenderContex::SwapBuffers()
	{
		glfwSwapBuffers(m_WindowHandle);
	}
}