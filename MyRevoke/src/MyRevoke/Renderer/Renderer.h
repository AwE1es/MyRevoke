#pragma once

#include "RenderCommand.h"
#include "RendererAPI.h"

#include "Shader.h"
#include "Cmaera.h"

namespace Revoke
{
	
	class Renderer
	{
	public:
		inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }

		static void Begin(Camera& camera);
		static void End();
		static void Init();

		static void Draw(const std::shared_ptr <Shader>& shader, const std::shared_ptr <VertexArray>& vertrexArray, const glm::mat4& transform = glm::mat4(1.0f));
	private:
		static Renderer s_RendererAPI;
		static glm::mat4 s_ViewProjectionMatrix;

	};

}