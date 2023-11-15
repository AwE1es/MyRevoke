#pragma once

#include "MyRevoke/Renderer/RendererAPI.h"


namespace Revoke
{
	class OpenGLRendererAPI : public RendererAPI
	{
	public:
		virtual void SetClearColor(const glm::vec4 color) override;
		virtual void Clear() override;
		virtual void DrawElements(const std::shared_ptr <VertexArray>& vertrexArray) override;
		virtual void EnableBlending() override;
	};
}