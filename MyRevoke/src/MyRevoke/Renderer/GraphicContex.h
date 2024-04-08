#pragma once
struct GLFWwindow;

namespace Revoke
{
	class RenderContex 
	{
	public:
		RenderContex(GLFWwindow* windowHandle);

		void Init();
		void SwapBuffers();
	private:
		GLFWwindow* m_WindowHandle;


	};
}