#include "rvpch.h"
#include "Renderer.h"

#include "MyRevoke/Renderer/Buffer.h"
#include "MyRevoke/Renderer/RenderCommand.h"
#include "Shader.h"
#include "Cmaera.h"

namespace Revoke
{
	glm::mat4 Renderer::s_ViewProjectionMatrix = {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f , 0.0f, 0.0f, 0.0f, 0.0f , 0.0f, 0.0f, 0.0f, 0.0f };

	void Renderer::Begin(Camera& camera)
	{
		
		s_ViewProjectionMatrix = camera.GetPVMatrix();
	}
	void Renderer::End()
	{
	}
	void Renderer::Draw(const std::shared_ptr <Shader>& shader, const std::shared_ptr<VertexArray>& vertrexArray, const glm::mat4& transform)
	{
		shader->Bind();
		shader->BindUniformMat4("u_PVmatrix", s_ViewProjectionMatrix);
		shader->BindUniformMat4("u_TransformMatrix", transform);
		vertrexArray->Bind();
		RenderCommand::DrawElements(vertrexArray);
	}
}
