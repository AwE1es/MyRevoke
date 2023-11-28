#include "rvpch.h"

#include "WIndowsInput.h"
#include "MyRevoke/Core/Application.h"

#include <GLFW/glfw3.h>

namespace Revoke
{
	Input* Input::s_Intance = new WindowsInput();;

	bool WindowsInput::IsKeyPressedImplementation(int keycode)
	{
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetCoreWindow());

		auto state = glfwGetKey(window, keycode);
		return state == GLFW_PRESS || state == GLFW_REPEAT;
	}

	bool WindowsInput::IsMousePressedImplementation(int mouseButton)
	{
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetCoreWindow());

		auto state = glfwGetMouseButton(window, mouseButton);
		return state == GLFW_PRESS;
	}

	float WindowsInput::GetMouseXImplementation()
	{
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetCoreWindow());

		double xPos, yPos;
		glfwGetCursorPos(window, &xPos, &yPos);
		return (float)xPos;
	}

	float WindowsInput::GetMouseYImplementation()
	{
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetCoreWindow());

		double xPos, yPos;
		glfwGetCursorPos(window, &xPos, &yPos);
		return (float)yPos;
	}


}
