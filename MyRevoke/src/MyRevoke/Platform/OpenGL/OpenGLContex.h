#pragma once

#include "MyRevoke/Renderer/GraphicsContex.h"

struct GLFWwindow;

namespace Revoke
{
	class OpenGLContex : public RenderContex
	{
	public:
		OpenGLContex(GLFWwindow* windowHandle);

		virtual void Init() override;
		virtual void SwapBuffers() override;
	private:
		GLFWwindow* m_WindowHandle;


	};
}