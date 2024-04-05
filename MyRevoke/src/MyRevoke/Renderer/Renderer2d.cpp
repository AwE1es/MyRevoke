#include "rvpch.h"
#include "Renderer2D.h"

#include "imgui.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp" 
#include <glm/gtc/type_ptr.hpp> 

namespace Revoke
{
	struct QuadVertex
	{
		glm::vec3 Position;
		glm::vec4 Color;
		glm::vec2 TexCoord;
		float TexIndex;


		int EntityId = 0;
	};

	struct Data2D
	{
		static const uint32_t MaxEllements = 20000;
		static const uint32_t MaxVertices = MaxEllements * 4;
		static const uint32_t MaxIndices = MaxEllements * 6;
		static const uint32_t MaxTextures = 32;

		Shared <VertexArray> QuadVA;
		Shared <VertexBuffer> QuadVB;

		Shared <VertexArray> TriangleVA;
		Shared <VertexBuffer> TriangleVB;

		Shared <Shader> Shader;
		Shared <Texture2D> WhiteTexture;

		uint32_t QuadIndexCount = 0;

		QuadVertex* QuadVertexBufferBase = nullptr;
		QuadVertex* QuadVertexBufferPointer = nullptr;

		std::array<Shared <Texture2D>, MaxTextures> Textures;
		uint32_t TextureIndex = 1;

		glm::vec4 QuadVertexPositions[4];

		Renderer2D::Stats Statistic;
	};

	static Data2D* s_Data;

	static const glm::vec4 whiteColor = { 1.0f, 1.0f, 1.0f, 1.0f };
	
	void Renderer2D::Init()
	{
		s_Data = new Data2D();

		QuadInit();
		//TriangleInit();

		s_Data->WhiteTexture = Texture2D::Create(1, 1);
		uint32_t whiteTextureData = 0xffffffff;
		s_Data->WhiteTexture->SetData(&whiteTextureData, sizeof(uint32_t));

		int32_t samplers[s_Data->MaxTextures];
		for (uint32_t i = 0; i < s_Data->MaxTextures; i++)
		{
			samplers[i] = i;
		}

		s_Data->Shader = Shader::Create("assets/Shaders/Main.shader");
		s_Data->Shader->Bind();
		s_Data->Shader->SetUniformIntArr("u_Textures", samplers, s_Data->MaxTextures);

		s_Data->Textures[0] = s_Data->WhiteTexture;

		s_Data->QuadVertexPositions[0] = { -0.5f, -0.5f, 0.0f, 1.0f };
		s_Data->QuadVertexPositions[1] = { 0.5f, -0.5f, 0.0f, 1.0f };
		s_Data->QuadVertexPositions[2] = { 0.5f,  0.5f, 0.0f, 1.0f };
		s_Data->QuadVertexPositions[3] = { -0.5f,  0.5f, 0.0f, 1.0f };
	}

	void Renderer2D::Shutdown()
	{
		delete[] s_Data->QuadVertexBufferBase;
	}

	void Renderer2D::Begin(const Camera& camera, const glm::mat4 transform)
	{

		glm::mat4 viewProj = camera.GetProjectionMatrix() * glm::inverse(transform);

		s_Data->Shader->Bind();
		s_Data->Shader->SetUniformMat4("u_PVmatrix", viewProj);

		s_Data->QuadIndexCount = 0;
		s_Data->QuadVertexBufferPointer = s_Data->QuadVertexBufferBase;
		s_Data->TextureIndex = 1;
	}

	void Renderer2D::Begin(const EditorCamera& camera)
	{

		glm::mat4 viewProj = camera.GetViewProjection();

		s_Data->Shader->Bind();
		s_Data->Shader->SetUniformMat4("u_PVmatrix", viewProj);

		s_Data->QuadIndexCount = 0;
		s_Data->QuadVertexBufferPointer = s_Data->QuadVertexBufferBase;
		s_Data->TextureIndex = 1;
	}

	void Renderer2D::End()
	{
		//Drawing

		uint32_t dataSize = (uint8_t*)s_Data->QuadVertexBufferPointer - (uint8_t*)s_Data->QuadVertexBufferBase;
		s_Data->QuadVB->InitData(s_Data->QuadVertexBufferBase, dataSize);

		for (uint32_t i = 0; i < s_Data->TextureIndex; i++)
		{
			s_Data->Textures[i]->Bind(i);
		}
		RenderCommand::DrawElements(s_Data->QuadVA, s_Data->QuadIndexCount);
		s_Data->Statistic.DrawCalls++;
	}

	//Color Draw
	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2 size, const glm::vec4& color, int entityID)
	{

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		DrawQuad(transform, color, entityID);

		if (s_Data->QuadIndexCount >= Data2D::MaxIndices)
			NewBatch();

/*
#ifdef RV_DEBUG
		s_Data->QuadVertexBufferPointer->Position = position;
		s_Data->QuadVertexBufferPointer->Color = color;
		s_Data->QuadVertexBufferPointer->TexCoord = { 0.0f, 0.0f };
		s_Data->QuadVertexBufferPointer->TexIndex = 0.0f;
		s_Data->QuadVertexBufferPointer++;

		s_Data->QuadVertexBufferPointer->Position = { position.x + size.x, position.y, 0.0f };
		s_Data->QuadVertexBufferPointer->Color = color;
		s_Data->QuadVertexBufferPointer->TexCoord = { 1.0f, 0.0f };
		s_Data->QuadVertexBufferPointer->TexIndex = 0.0f;
		s_Data->QuadVertexBufferPointer++;

		s_Data->QuadVertexBufferPointer->Position = { position.x + size.x, position.y + size.y, 0.0f };
		s_Data->QuadVertexBufferPointer->Color = color;
		s_Data->QuadVertexBufferPointer->TexCoord = { 1.0f, 1.0f };
		s_Data->QuadVertexBufferPointer->TexIndex = 0.0f;
		s_Data->QuadVertexBufferPointer++;

		s_Data->QuadVertexBufferPointer->Position = { position.x , position.y + size.y, 0.0f };
		s_Data->QuadVertexBufferPointer->Color = color;
		s_Data->QuadVertexBufferPointer->TexCoord = { 0.0f, 1.0f };
		s_Data->QuadVertexBufferPointer->TexIndex = 0.0f;
		s_Data->QuadVertexBufferPointer++;

		s_Data->QuadIndexCount += 6;

		s_Data->Statistic.QuadCount++;
#endif 
*/
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2 size, const glm::vec4& color, int entityID)
	{
		DrawQuad({ position.x, position.y, 0.0f }, size, color, entityID);
	}

	//Texture Draw
	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2 size, const Shared<Texture2D>& texture, int entityID)
	{

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		DrawQuad(transform, texture, entityID);

	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2 size, const Shared<Texture2D>& texture, int entityID)
	{
		DrawQuad({ position.x, position.y, 0.0f }, size, texture, entityID);
	}

	void Renderer2D::DrawQuad(const glm::mat4& transform, const glm::vec4& color, int entityID)
	{

		constexpr size_t quadVertexCount = 4;
		const float textureIndex = 0.0f; // White Texture
		constexpr glm::vec2 textureCoords[] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };

		if (s_Data->QuadIndexCount >= Data2D::MaxIndices)
			NewBatch();

		for (size_t i = 0; i < quadVertexCount; i++)
		{
			s_Data->QuadVertexBufferPointer->Position = transform * s_Data->QuadVertexPositions[i];
			s_Data->QuadVertexBufferPointer->Color = color;
			s_Data->QuadVertexBufferPointer->TexCoord = textureCoords[i];
			s_Data->QuadVertexBufferPointer->TexIndex = textureIndex;
			s_Data->QuadVertexBufferPointer->EntityId = entityID;
			s_Data->QuadVertexBufferPointer++;
		}

		s_Data->QuadIndexCount += 6;

		s_Data->Statistic.QuadCount++;
	}

	void Renderer2D::DrawQuad(const glm::mat4& transform, const Shared<Texture2D>& texture, int entityID)
	{
		constexpr size_t quadVertexCount = 4;
		constexpr glm::vec2 textureCoords[] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };

		if (s_Data->QuadIndexCount >= Data2D::MaxIndices)
			NewBatch();

		float textureIndex = 0.0f;

		for (uint32_t i = 1; i < s_Data->TextureIndex; i++)
		{
			if (s_Data->Textures[i]->GetID() == texture->GetID())
			{
				textureIndex = (float)i;
				break;
			}
		}

		if (textureIndex == 0.0f)
		{
			if (s_Data->TextureIndex >= Data2D::MaxTextures)
				NewBatch();
			textureIndex = (float)s_Data->TextureIndex;
			s_Data->Textures[s_Data->TextureIndex] = texture;
			s_Data->TextureIndex++;
		}

		for (size_t i = 0; i < quadVertexCount; i++)
		{
			s_Data->QuadVertexBufferPointer->Position = transform * s_Data->QuadVertexPositions[i];
			s_Data->QuadVertexBufferPointer->Color = whiteColor;
			s_Data->QuadVertexBufferPointer->TexCoord = textureCoords[i];
			s_Data->QuadVertexBufferPointer->TexIndex = textureIndex;
			s_Data->QuadVertexBufferPointer->EntityId = entityID;
			s_Data->QuadVertexBufferPointer++;
		}

		s_Data->QuadIndexCount += 6;

		s_Data->Statistic.QuadCount++;
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


	void Renderer2D::DrawTriangle(const glm::vec3& position, const glm::vec2 size, const Shared<Texture2D>& texture)
	{
		
		s_Data->QuadIndexCount += 6;

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		s_Data->Shader->SetUniformMat4("u_TransformMatrix", transform);
		s_Data->Shader->SetUniformFloat("u_TextureScale", { 1.0f });
		s_Data->Shader->SetUniformVec4("u_Color", { 1.0f, 1.0f, 1.0f, 1.0f });

		texture->Bind(0);
		s_Data->TriangleVA->Bind();
		RenderCommand::DrawElements(s_Data->TriangleVA);
	}

	void Renderer2D::DrawTriangle(const glm::vec2& position, const glm::vec2 size, const Shared<Texture2D>& texture)
	{
		DrawTriangle({ position.x, position.y, 0.0f }, size, texture);
	}

	void Renderer2D::DrawSprite(const glm::mat4& transform, SpriteRendererComponent& sprite, int entityID)
	{
		if (sprite.Texture)
		{
			DrawQuad(transform, sprite.Texture, entityID);
		}
		else
		{
			DrawQuad(transform, sprite.Color, entityID);
		}
	}

	void Renderer2D::QuadInit()
	{
		s_Data->QuadVA = VertexArray::Create();

		s_Data->QuadVB = VertexBuffer::Create(s_Data->MaxVertices * sizeof(QuadVertex));

		BufferLayout quadLayout = {
			{ShaderDataTypes::Float3, "a_Position", false},
			{ShaderDataTypes::Float4, "a_Color", false},
			{ShaderDataTypes::Float2, "a_TexCoord", false},
			{ShaderDataTypes::Float,  "a_TexIndex", false},
			{ShaderDataTypes::Int,  "a_EntityID", false}
		};
		s_Data->QuadVB->SetLayout(quadLayout);

		s_Data->QuadVA->AddVertexBuffer(s_Data->QuadVB);

		s_Data->QuadVertexBufferBase = new QuadVertex[s_Data->MaxVertices];

		uint32_t* quadIndices = new uint32_t[s_Data->MaxIndices];
		

		uint32_t offset = 0;
		for (uint32_t i = 0; i < s_Data->MaxIndices; i += 6)
		{
			quadIndices[i + 0] = offset + 0;
			quadIndices[i + 1] = offset + 1;
			quadIndices[i + 2] = offset + 2;

			quadIndices[i + 3] = offset + 2;
			quadIndices[i + 4] = offset + 3;
			quadIndices[i + 5] = offset + 0;

			offset += 4;
		}

		Shared<IndexBuffer> QuadIndexBuffer;
		QuadIndexBuffer = IndexBuffer::Create(quadIndices, s_Data->MaxIndices);
		s_Data->QuadVA->SetIndexBuffer(QuadIndexBuffer);
		delete[] quadIndices;
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

		s_Data->TriangleVB = VertexBuffer::Create(Vertices, sizeof(Vertices));

		BufferLayout gridLayout = {
			{ShaderDataTypes::Float3, "a_Position", false},
			{ShaderDataTypes::Float2, "a_TexCoord", false}
		};
		s_Data->TriangleVB->SetLayout(gridLayout);

		s_Data->TriangleVA->AddVertexBuffer(s_Data->TriangleVB);

		uint32_t indices[3 * 2] = { 0, 1, 2 };
		Shared<IndexBuffer> IndexBuffer;
		IndexBuffer = IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t));
		s_Data->TriangleVA->SetIndexBuffer(IndexBuffer);
	}

	void Renderer2D::NewBatch()
	{
		End();
		s_Data->QuadIndexCount = 0;
		s_Data->QuadVertexBufferPointer = s_Data->QuadVertexBufferBase;
		s_Data->TextureIndex = 1;
	}

	Renderer2D::Stats Renderer2D::GetStats()
	{
		return s_Data->Statistic;
	}

	void Renderer2D::ResetStatistics()
	{
		memset(&s_Data->Statistic, 0, sizeof(Stats));
	}

}