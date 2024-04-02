#pragma once
#include "RenderCommand.h"
#include "RendererAPI.h"

#include "Shader.h"
#include "Cmaera.h"
#include "Texture.h"
#include "EditorCamera.h"
#include "MyRevoke/Scene/Components.h"

#include "glm/glm.hpp"

namespace Revoke
{

	class Renderer2D
	{
	public:
		inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }

		static void Init();
		static void Shutdown();

		static void Begin(const Camera& camera, const glm::mat4 transform);
		static void Begin(const EditorCamera& camera);
		static void End();

		

		static void DrawQuad(const glm::vec3& position, const glm::vec2 size, const glm::vec4& color, int entityID);
		static void DrawQuad(const glm::vec2& position, const glm::vec2 size, const glm::vec4& color, int entityID);
		static void DrawQuad(const glm::vec3& position, const glm::vec2 size, const Shared<Texture2D>& texture, int entityID);
		static void DrawQuad(const glm::vec2& position, const glm::vec2 size, const Shared<Texture2D>& texture, int entityID);

		static void DrawQuad(const glm::mat4& transform, const glm::vec4& color, int entityID);
		static void DrawQuad(const glm::mat4& transform, const Shared<Texture2D>& texture, int entityID);

		static void DrawTriangle(const glm::vec3& position, const glm::vec2 size, const glm::vec4& color);			  //TODO
		static void DrawTriangle(const glm::vec2& position, const glm::vec2 size, const glm::vec4& color);			  //TODO
		static void DrawTriangle(const glm::vec3& position, const glm::vec2 size, const Shared<Texture2D>& texture);  //TODO
		static void DrawTriangle(const glm::vec2& position, const glm::vec2 size, const Shared<Texture2D>& texture);  //TODO

		static void DrawSprite(const glm::mat4& transform, SpriteRendererComponent& sprite, int entityID);

		static void QuadInit();
		static void TriangleInit();// TODO
	private:
		static void NewBatch();
	public:

		// Stats
		struct Stats
		{
			uint32_t DrawCalls = 0;
			uint32_t QuadCount = 0;

			uint32_t GetTotalVertexCount() { return QuadCount * 4; }
			uint32_t GetTotalIndexCount() { return QuadCount * 6; }
		};

		static Stats GetStats();
		static void ResetStatistics();

	};

}