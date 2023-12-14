#pragma once

#include "MyRevoke/Renderer/RendererAPI.h"


namespace Revoke
{
	class OpenGLRendererAPI : public RendererAPI
	{
	public:
		virtual void SetClearColor(const glm::vec4 color) override;
		virtual void Clear() override;
		virtual void DrawElements(const Shared<VertexArray>& vertrexArray, uint32_t count = 0) override;
		virtual void EnableBlending() override;
		virtual void WindowResize(float width, float height) override;

		virtual void Init() override;
	};
}