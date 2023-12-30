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
	
	void Renderer::Begin(Camera& camera)
	{
		//glm::mat4 viewProj = camera.GetProjection() * glm::inverse(transform);

		//s_Data.TextureShader->Bind();
		//s_Data.TextureShader->SetMat4("u_ViewProjection", viewProj);

		//s_Data.QuadIndexCount = 0;
		//s_Data.QuadVertexBufferPtr = s_Data.QuadVertexBufferBase;

		//s_Data.TextureSlotIndex = 1;
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
		std::static_pointer_cast<OpenGLShader>(shader)->BindUniformMat4("u_TransformMatrix", transform);
		vertrexArray->Bind();
		RenderCommand::DrawElements(vertrexArray);
	}
}
