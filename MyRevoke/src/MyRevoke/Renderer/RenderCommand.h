#pragma once

#include "glm/glm.hpp"
#include "RendererAPI.h"

namespace Revoke
{

	class RenderCommand
	{
	public:
		inline static void ClearColor(const glm::vec4 color) { s_RendererAPI->SetClearColor(color); }
		inline static void Clear() { s_RendererAPI->Clear(); }
		inline static void EnableBlending() { s_RendererAPI->EnableBlending(); }
		inline static void OnResize(float width, float height) { s_RendererAPI->WindowResize(width,  height); };

		inline static void Init() { s_RendererAPI->Init(); }
		 
		inline static void DrawElements(const Shared <VertexArray>& vertrexArray, uint32_t count = 0) { s_RendererAPI->DrawElements(vertrexArray, count); }
	private:
		static RendererAPI* s_RendererAPI;
	};

}