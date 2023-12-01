#include "rvpch.h"
#include "Renderer.h"

#include "MyRevoke/Renderer/Buffer.h"
#include "MyRevoke/Renderer/RenderCommand.h"
#include "MyRevoke/Platform/OpenGL/OpenGlShader.h"
#include "Shader.h"
#include "Cmaera.h"
#include "Renderer2D.h"

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

	void Renderer::Init()
	{
		RenderCommand::Init();
		Renderer2D::Init();
	}
	void Renderer::Draw(const Shared<Shader>& shader, const Shared<VertexArray>& vertrexArray, const glm::mat4& transform)
	{
		shader->Bind();
		std::static_pointer_cast<OpenGLShader>(shader)->BindUniformMat4("u_PVmatrix", s_ViewProjectionMatrix);
		std::static_pointer_cast<OpenGLShader>(shader)->BindUniformMat4("u_TransformMatrix", transform);
		vertrexArray->Bind();
		RenderCommand::DrawElements(vertrexArray);
	}
}
