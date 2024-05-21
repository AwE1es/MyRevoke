#include "rvpch.h"

#include "Input.h"
#include "Application.h"

#include <GLFW/glfw3.h>

namespace Revoke
{

	GLFWwindow* g_Window;

	void Input::SetInputWindow()
	{
		g_Window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetCoreWindow());
	}
	
	bool Input::IsKeyPressed(int key)
	{
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetCoreWindow());

		auto stateOfKey = glfwGetKey(window, key);
		return stateOfKey == GLFW_PRESS || stateOfKey == GLFW_REPEAT;
	}

	bool Input::IsKeyPressedScr(int key)
	{
		auto stateOfKey = glfwGetKey(g_Window, key);
		return stateOfKey == GLFW_PRESS || stateOfKey == GLFW_REPEAT;
	}

	bool Input::IsMousePressed(int mouseButton)
	{
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetCoreWindow());

		auto stateOfMouse = glfwGetMouseButton(window, mouseButton);
		return stateOfMouse == GLFW_PRESS;
	}

	float Input::GetMouseX()
	{
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetCoreWindow());

		double xPosition, yPosition;
		glfwGetCursorPos(window, &xPosition, &yPosition);
		return (float)xPosition;
	}

	float Input::GetMouseY()
	{
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetCoreWindow());

		double xPos, yPos;
		glfwGetCursorPos(window, &xPos, &yPos);
		return (float)yPos;
	}

	std::pair<float, float> Input::GetMousePosition()
	{
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetCoreWindow());

		double xPosition, yPosition;
		glfwGetCursorPos(window, &xPosition, &yPosition);

		return std::make_pair((float)xPosition, (float)yPosition);
	}
}