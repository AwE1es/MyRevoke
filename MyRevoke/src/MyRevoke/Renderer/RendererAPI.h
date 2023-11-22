#pragma once

#include "glm/glm.hpp"

#include "MyRevoke/Renderer/Buffer.h"

namespace Revoke
{

	class RendererAPI
	{
	
	public:
		enum class API
		{
			None = 0,
			OpenGL = 1
		};
	public:
		virtual void SetClearColor(const glm::vec4 color) = 0;
		virtual void Clear() = 0;
		virtual void DrawElements(const std::shared_ptr <VertexArray>& vertrexArray) = 0;
		virtual void EnableBlending() = 0;
		virtual void Init() = 0;

		inline static API GetAPI() { return s_API; };
	private:
		static API s_API;
	};
}