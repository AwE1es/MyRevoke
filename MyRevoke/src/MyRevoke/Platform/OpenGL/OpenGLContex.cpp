#include "rvpch.h"

#include "OpenGLContex.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

Revoke::OpenGLContex::OpenGLContex(GLFWwindow* windowHandle)
	: m_WindowHandle(windowHandle) 
{
	RV_CORE_ASSERT(windowHandle, "Window handle is null :(");
}

void Revoke::OpenGLContex::Init()
{
	glfwMakeContextCurrent(m_WindowHandle);
	int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
	RV_CORE_ASSERT(status, "Failed to initialize GLAD");
}

void Revoke::OpenGLContex::SwapBuffers()
{
	glfwSwapBuffers(m_WindowHandle);
}
