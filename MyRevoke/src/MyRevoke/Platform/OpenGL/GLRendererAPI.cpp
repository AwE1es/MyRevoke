#include "rvpch.h"
#include "GLRendererAPI.h"

#include "glad/glad.h"

namespace Revoke {

	void OpenGLRendererAPI::SetClearColor(const glm::vec4 color)
	{
		glClearColor(color.r, color.g, color.b, color.a);
	}

	void OpenGLRendererAPI::Clear()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void OpenGLRendererAPI::DrawElements(const Shared<VertexArray>& vertrexArray, uint32_t count )
	{
		uint32_t _count = count ? vertrexArray->GetIndexBuffer()->GetCount() : count;
		glDrawElements(GL_TRIANGLES, _count, GL_UNSIGNED_INT, nullptr);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
	void OpenGLRendererAPI::EnableBlending()
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glEnable(GL_DEPTH_TEST);
	}
	void OpenGLRendererAPI::WindowResize(float width, float height)
	{
		glViewport(0, 0, width, height);
	}
	void OpenGLRendererAPI::Init()
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}
}
