#include "rvpch.h"
#include "Renderer2D.h"

#include "imgui.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp" 
#include <glm/gtc/type_ptr.hpp> 

namespace Revoke
{

	struct Data2D
	{
		Shared <VertexArray> QuadVA;
		Shared <VertexArray> TriangleVA;
		Shared <Shader> Shader;
		Shared <Texture> WhiteTexture;
	};

	static Data2D* s_Data;

	void Renderer2D::Init()
	{
		s_Data = new Data2D();

		QuadInit();
		TriangleInit();

		s_Data->WhiteTexture = Texture2D::Create(1, 1);
		uint32_t whiteTextureData = 0xffffffff;
		s_Data->WhiteTexture->SetData(&whiteTextureData, sizeof(uint32_t));

		s_Data->Shader = Shader::Create("Shaders/Main.shader");
		s_Data->Shader->Bind();
		s_Data->Shader->SetUniformInt("u_Texture", 0);
	}

	void Renderer2D::Shutdown()
	{
		delete s_Data;
	}

	void Renderer2D::Begin(Camera& camera)
	{
		s_Data->Shader->Bind();
		s_Data->Shader->SetUniformMat4("u_PVmatrix", camera.GetPVMatrix());

	}

	void Renderer2D::End()
	{

	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2 size, const glm::vec4& color)
	{
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		s_Data->Shader->SetUniformMat4("u_TransformMatrix", transform);
		s_Data->Shader->SetUniformVec4("u_Color", color);
		s_Data->Shader->SetUniformFloat("u_TextureScale", { 1.0f });

		s_Data->WhiteTexture->Bind(0);
		s_Data->QuadVA->Bind();
		RenderCommand::DrawElements(s_Data->QuadVA);
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2 size, const glm::vec4& color)
	{
		DrawQuad({ position.x, position.y, 0.0f }, size, color);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2 size, const Shared<Texture>& texture)
	{
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		s_Data->Shader->SetUniformMat4("u_TransformMatrix", transform);
		s_Data->Shader->SetUniformFloat("u_TextureScale", { 1.0f });
		s_Data->Shader->SetUniformVec4("u_Color", { 1.0f, 1.0f, 1.0f, 1.0f });

		texture->Bind(0);
		s_Data->QuadVA->Bind();
		RenderCommand::DrawElements(s_Data->QuadVA);
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2 size, const Shared<Texture>& texture)
	{
		DrawQuad({ position.x, position.y, 0.0f }, size, texture);
	}

	void Renderer2D::DrawTriangle(const glm::vec3& position, const glm::vec2 size, const glm::vec4& color)
	{

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		s_Data->Shader->SetUniformMat4("u_TransformMatrix", transform);
		s_Data->Shader->SetUniformVec4("u_Color", color);
		s_Data->Shader->SetUniformFloat("u_TextureScale", { 1.0f });

		s_Data->WhiteTexture->Bind(0);
		s_Data->TriangleVA->Bind();
		RenderCommand::DrawElements(s_Data->TriangleVA);
	}

	void Renderer2D::DrawTriangle(const glm::vec2& position, const glm::vec2 size, const glm::vec4& color)
	{
		DrawTriangle({ position.x, position.y, 0.0f }, size, color);
	}

	void Renderer2D::DrawTriangle(const glm::vec3& position, const glm::vec2 size, const Shared<Texture>& texture)
	{
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		s_Data->Shader->SetUniformMat4("u_TransformMatrix", transform);
		s_Data->Shader->SetUniformFloat("u_TextureScale", { 1.0f });
		s_Data->Shader->SetUniformVec4("u_Color", { 1.0f, 1.0f, 1.0f, 1.0f });

		texture->Bind(0);
		s_Data->TriangleVA->Bind();
		RenderCommand::DrawElements(s_Data->TriangleVA);
	}

	void Renderer2D::DrawTriangle(const glm::vec2& position, const glm::vec2 size, const Shared<Texture>& texture)
	{
		DrawTriangle({ position.x, position.y, 0.0f }, size, texture);
	}

	void Renderer2D::QuadInit()
	{
		s_Data->QuadVA = VertexArray::Create();


		float Vertices[5 * 4] = {
			-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
			 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
			 0.5f,  0.5f, 0.0f,	1.0f, 1.0f,
			-0.5f,  0.5f, 0.0f, 0.0f, 1.0f
		};
		//	Vertex Buffer
		Shared<VertexBuffer> VertexBuffer;
		VertexBuffer = VertexBuffer::Create(Vertices, sizeof(Vertices));

		BufferLayout gridLayout = {
			{ShaderDataTypes::Float3, "Position", false},
			{ShaderDataTypes::Float2, "a_TexCoord", false}
		};
		VertexBuffer->SetLayout(gridLayout);

		s_Data->QuadVA->AddVertexBuffer(VertexBuffer);

		uint32_t indecies[6] = { 0, 1, 2, 2, 3, 0 };
		Shared<IndexBuffer> IndexBuffer;
		IndexBuffer = IndexBuffer::Create(indecies, sizeof(indecies) / sizeof(uint32_t));
		s_Data->QuadVA->SetIndexBuffer(IndexBuffer);
	}

	void Renderer2D::TriangleInit()
	{
		s_Data->TriangleVA = VertexArray::Create();

		float Vertices[3 * 3 * 2] = {
			-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
			 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
			 0.0f,  0.5f, 0.0f, 0.5f, 1.0f
		};
		// Vertex Buffer
		Shared<VertexBuffer> VertexBuffer;
		VertexBuffer = VertexBuffer::Create(Vertices, sizeof(Vertices));

		BufferLayout gridLayout = {
			{ShaderDataTypes::Float3, "Position", false},
			{ShaderDataTypes::Float2, "a_TexCoord", false}
		};
		VertexBuffer->SetLayout(gridLayout);

		s_Data->TriangleVA->AddVertexBuffer(VertexBuffer);

		uint32_t indices[3 * 2] = { 0, 1, 2 };
		Shared<IndexBuffer> IndexBuffer;
		IndexBuffer = IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t));
		s_Data->TriangleVA->SetIndexBuffer(IndexBuffer);
	}

}