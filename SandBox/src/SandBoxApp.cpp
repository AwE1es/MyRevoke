#include <MyRevoke.h>

#include "MyRevoke/Platform/OpenGL/OpenGlShader.h"

#include "imgui.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp" 
#include <glm/gtc/type_ptr.hpp> 

class TestLayer : public Revoke::Layer
{
public:
	TestLayer()
		:Layer("Testing"), m_Camera(-1.6f, 1.6f, -0.9f, 0.9f), m_CameraPos(0.0f)
	{
			m_MeshColor = { 0.8f, 0.8f, 0.2f, 1.0f };
			m_MeshLocation = { 0.25f, -0.25f };
			m_MeshScale = { 0.5f, 0.5f };
			m_VertexArray = Revoke::VertexArray::Create();


			//	Index Buffer
			Revoke::Shared<Revoke::IndexBuffer> indexBuffer;
			float vertices[3 * 3] = {
				-0.5f, -0.5f, 0.0f,
				 0.5f, -0.5f, 0.0f,
				 0.0f,  0.5f, 0.0f
			};
			//	Vertex Buffer
			Revoke::Shared<Revoke::VertexBuffer> vertexBuffer;
			vertexBuffer = Revoke::VertexBuffer::Create(vertices, sizeof(vertices));

			Revoke::BufferLayout layout = {
				{Revoke::ShaderDataTypes::Float3, "Position", false},
			};
			vertexBuffer->SetLayout(layout);
			m_VertexArray->AddVertexBuffer(vertexBuffer);

			uint32_t indecies[3] = { 0, 1, 2 };
			indexBuffer = Revoke::IndexBuffer::Create(indecies, 3);
			m_VertexArray->SetIndexBuffer(indexBuffer);

		
		//*****************************************************************************
		

			m_GridVertexArray = Revoke::VertexArray::Create();

			//	Index Buffer
			Revoke::Shared<Revoke::IndexBuffer> gridIndexBuffer;
			float gridVertices[5 * 4] = {
				-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
				 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
				 0.5f,  0.5f, 0.0f,	1.0f, 1.0f,
				-0.5f,  0.5f, 0.0f, 0.0f, 1.0f
			};
			//	Vertex Buffer
			Revoke::Shared<Revoke::VertexBuffer> gridVertexBuffer;
			gridVertexBuffer = Revoke::VertexBuffer::Create(gridVertices, sizeof(gridVertices));

			Revoke::BufferLayout gridLayout = {
				{Revoke::ShaderDataTypes::Float3, "Position", false},
				{Revoke::ShaderDataTypes::Float2, "TexCoord", false}
			};
			gridVertexBuffer->SetLayout(gridLayout);

			m_GridVertexArray->AddVertexBuffer(gridVertexBuffer);

			uint32_t gridIndecies[6] = { 0, 1, 2, 2, 3, 0 };
			gridIndexBuffer = Revoke::IndexBuffer::Create(gridIndecies, sizeof(gridIndecies)/sizeof(uint32_t));
			m_GridVertexArray->SetIndexBuffer(gridIndexBuffer);

			
			m_ShaderLib.LoadShader("Grid Shader", "Shaders/Grid.shader");

			//*****************************************************************************
	
			m_ShaderLib.LoadShader("Texture Shader", "Shaders/Texture.shader");

			m_Texture2D = Revoke::Texture2D::Create("Textures/Triangle_Texture.png");

			auto textureShader = m_ShaderLib.GetShaderByName("Texture Shader");
			textureShader->Bind();
			std::dynamic_pointer_cast<Revoke::OpenGLShader>(textureShader)->BindUniformInt("u_Texture", 0);

	}
	void OnUpdate(Revoke::Timestep deltaTime) override
	{


		if (Revoke::Input::IsKeyPressed(RV_KEY_LEFT))
		{
			m_CameraPos.x -= m_CamSpeed * deltaTime.GetSeconds();
		}
		if (Revoke::Input::IsKeyPressed(RV_KEY_RIGHT))
		{
			m_CameraPos.x += m_CamSpeed * deltaTime.GetSeconds();
		}

		if (Revoke::Input::IsKeyPressed(RV_KEY_UP))
		{
			m_CameraPos.y += m_CamSpeed * deltaTime.GetSeconds();
		}
		if (Revoke::Input::IsKeyPressed(RV_KEY_DOWN))
		{
			m_CameraPos.y -= m_CamSpeed * deltaTime.GetSeconds();
		}

		
		glm::mat4 objTrasnform = glm::translate(glm::mat4(1.0f), glm::vec3(m_MeshLocation, 0.0f));
		glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));

		Revoke::RenderCommand::ClearColor({ 0.2f, 0.2f, 0.2f, 1.0f });
		Revoke::RenderCommand::Clear();
		Revoke::RenderCommand::EnableBlending();

		m_Camera.SetCameraPropeties(m_CameraPos);
		Revoke::Renderer::Begin(m_Camera);

		glm::vec4 darker = {1.0f, 1.0f, 1.0f, 0.2f};
		glm::vec4 brighter = { 1.0f, 1.0f, 1.0f, 0.1f };

		auto gridShader = m_ShaderLib.GetShaderByName("Grid Shader");

		for (int y = -10; y < 10; y++)
		{
			for (int x = -20; x < 20; x++)
			{
				glm::vec3 pos(x * 0.11f, y * 0.11f, 0.0f);
				glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos) * scale;
				if (x % 2 != 0)
				{
					std::static_pointer_cast<Revoke::OpenGLShader>(gridShader)->BindMaterial("u_Color", darker);
				}
				else
				{
					std::static_pointer_cast<Revoke::OpenGLShader>(gridShader)->BindMaterial("u_Color", brighter);
				}
				Revoke::Renderer::Draw(gridShader, m_GridVertexArray, transform);
			}
		}

		scale = glm::scale(glm::mat4(1.0f), glm::vec3(m_MeshScale, 0.0f));
		//std::static_pointer_cast<Revoke::OpenGLShader>(m_Shader)->Bind();
		//std::static_pointer_cast<Revoke::OpenGLShader>(m_Shader)->BindMaterial("u_Color1", m_MeshColor);
		//Revoke::Renderer::Draw(m_Shader, m_VertexArray, objTrasnform * scale);
		 
		auto textureShader = m_ShaderLib.GetShaderByName("Texture Shader");
		textureShader->Bind();
		m_Texture2D->Bind(0); 
		Revoke::Renderer::Draw(textureShader, m_GridVertexArray, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));

		Revoke::Renderer::End();

	}

	void OnImGuiDraw()
	{

	}

	void OnEvent(Revoke::Event& e) override
	{
		
	}

	private:
	//Objects
	Revoke::Shared <Revoke::VertexArray> m_VertexArray;

	// Grid
	Revoke::Shared <Revoke::VertexArray> m_GridVertexArray;

	//Textures
	Revoke::Shared <Revoke::Texture> m_Texture2D;

	Revoke::ShaderLibrary m_ShaderLib;

	Revoke::Camera m_Camera;
	glm::vec3 m_CameraPos;

	float m_Top = 0.9f, m_Down = -0.9f, m_Right = 1.6f, m_Left = -1.6f;

	float m_CamSpeed = 1.0f;

	
	glm::vec4 m_MeshColor;
	glm::vec2 m_MeshLocation;
	glm::vec2 m_MeshScale;
};

class SandBox : public Revoke::Application
{
public:
	SandBox()
	{
		PushLayer(new TestLayer());
	}
	~SandBox()
	{

	}

private:

};

Revoke::Application* Revoke::CreateApplication()
{
	return new SandBox();
}