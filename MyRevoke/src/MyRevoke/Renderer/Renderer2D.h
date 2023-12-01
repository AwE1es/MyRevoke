#pragma once
#include "RenderCommand.h"
#include "RendererAPI.h"


#include "Shader.h"
#include "Cmaera.h"
#include "Texture.h"

#include "glm/glm.hpp"

namespace Revoke
{

	class Renderer2D
	{
	public:
		inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }

		static void Init();
		static void Shutdown();

		static void Begin(Camera& camera);
		static void End();

		static void DrawQuad(const glm::vec3& position, const glm::vec2 size, const glm::vec4& color);
		static void DrawQuad(const glm::vec2& position, const glm::vec2 size, const glm::vec4& color);
		static void DrawQuad(const glm::vec3& position, const glm::vec2 size, const Shared<Texture>& texture);
		static void DrawQuad(const glm::vec2& position, const glm::vec2 size, const Shared<Texture>& texture);

		static void DrawTriangle(const glm::vec3& position, const glm::vec2 size, const glm::vec4& color);
		static void DrawTriangle(const glm::vec2& position, const glm::vec2 size, const glm::vec4& color);
		static void DrawTriangle(const glm::vec3& position, const glm::vec2 size, const Shared<Texture>& texture);
		static void DrawTriangle(const glm::vec2& position, const glm::vec2 size, const Shared<Texture>& texture);

		static void QuadInit();
		static void TriangleInit();

	};

}