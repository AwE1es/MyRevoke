#include <MyRevoke.h>

#include "glm/glm.hpp"
#include "imgui.h"
#include "glm/gtc/matrix_transform.hpp"

class TestLayer : public Revoke::Layer
{
public:
	TestLayer()
		:Layer("Testing"), m_Camera(-1.6f, 1.6f, -0.9f, 0.9f), m_CameraPos(0.0f)
	{
		
			m_VertexArray.reset(Revoke::VertexArray::Create());


			//	Index Buffer
			std::shared_ptr<Revoke::IndexBuffer> indexBuffer;
			float vertices[3 * 7] = {
				-0.5f, -0.5f, 0.0f, 0.8f, 0.2f, 0.8f, 1.0f,
				 0.5f, -0.5f, 0.0f, 0.2f, 0.3f, 0.8f, 1.0f,
				 0.0f,  0.5f, 0.0f, 0.8f, 0.8f, 0.2f, 1.0f
			};
			//	Vertex Buffer
			std::shared_ptr<Revoke::VertexBuffer> vertexBuffer;
			vertexBuffer.reset(Revoke::VertexBuffer::Create(vertices, sizeof(vertices)));

			Revoke::BufferLayout layout = {
				{Revoke::ShaderDataTypes::Float3, "Position", false},
				{Revoke::ShaderDataTypes::Float4, "Color"}
			};
			vertexBuffer->SetLayout(layout);
			m_VertexArray->AddVertexBuffer(vertexBuffer);

			uint32_t indecies[3] = { 0, 1, 2 };
			indexBuffer.reset(Revoke::IndexBuffer::Create(indecies, 3));
			m_VertexArray->SetIndexBuffer(indexBuffer);

		
		//*****************************************************************************
		

			m_GridVertexArray.reset(Revoke::VertexArray::Create());

			//	Index Buffer
			std::shared_ptr<Revoke::IndexBuffer> gridIndexBuffer;
			float gridVertices[3 * 4] = {
				-0.5f, -0.5f, 0.0f,
				 0.5f, -0.5f, 0.0f,
				 0.5f,  0.5f, 0.0f,
				-0.5f,  0.5f, 0.0f
			};
			//	Vertex Buffer
			std::shared_ptr<Revoke::VertexBuffer> gridVertexBuffer;
			gridVertexBuffer.reset(Revoke::VertexBuffer::Create(gridVertices, sizeof(gridVertices)));

			Revoke::BufferLayout gridLayout = {
				{Revoke::ShaderDataTypes::Float3, "Position", false}
			};
			gridVertexBuffer->SetLayout(gridLayout);

			m_GridVertexArray->AddVertexBuffer(gridVertexBuffer);

			uint32_t gridIndecies[6] = { 0, 1, 2, 2, 3, 0 };
			gridIndexBuffer.reset(Revoke::IndexBuffer::Create(gridIndecies, 6));
			m_GridVertexArray->SetIndexBuffer(gridIndexBuffer);

			std::string gridVertexSrc = R"(
			
			#version 330 core
			
			layout(location = 0) in vec3 a_Pos;

			uniform mat4 u_PVmatrix;	
			uniform mat4 u_TransformMatrix;

			out vec3 v_Position;
			out vec4 v_Color;
	

			void main()
			{
				v_Position = a_Pos;
				v_Color = vec4(1.0, 1.0, 1.0, 0.1 );
				gl_Position = u_PVmatrix * u_TransformMatrix * vec4(a_Pos, 1.0);
			}
		)";

			std::string gridFragmentSrc = R"(
			
			#version 330 core
			
			layout(location = 0) out vec4 o_Color;
			in vec3 v_Position;
			in vec4 v_Color;

			void main()
			{
				o_Color = v_Color;
			}
		)";

			m_GridShader.reset(new Revoke::Shader(gridVertexSrc, gridFragmentSrc));


			std::string vertexSrc = R"(
			
			#version 330 core
			
			layout(location = 0) in vec3 a_Pos;
			layout(location = 1) in vec4 a_Color;

			uniform mat4 u_PVmatrix;	
			uniform mat4 u_TransformMatrix;

			out vec3 v_Position;
			out vec4 v_Color;
	

			void main()
			{
				v_Position = a_Pos;
				v_Color = a_Color;
				gl_Position = u_PVmatrix * u_TransformMatrix * vec4(a_Pos, 1.0);
			}
		)";

			std::string fragmentSrc = R"(
			
			#version 330 core
			
			layout(location = 0) out vec4 o_Color;
			in vec3 v_Position;
			in vec4 v_Color;

			void main()
			{
				o_Color = v_Color;
			}
		)";
			m_Shader.reset(new Revoke::Shader(vertexSrc, fragmentSrc));
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

		glm::mat4 objTrasnform = glm::translate(glm::mat4(1.0f), { 0.25f, -0.25f, 0.0f });
		glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));

		Revoke::RenderCommand::ClearColor({ 0.2f, 0.2f, 0.2f, 1.0f });
		Revoke::RenderCommand::Clear();
		Revoke::RenderCommand::EnableBlending();

		m_Camera.SetCameraPropeties(m_CameraPos);
		Revoke::Renderer::Begin(m_Camera);

		for (int y = -10; y < 10; y++)
		{
			for (int x = -20; x < 20; x++)
			{
				glm::vec3 pos(x * 0.11f, y * 0.11f, 0.0f);
				glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos) * scale;
				Revoke::Renderer::Draw(m_GridShader, m_GridVertexArray, transform);
			}
		}
		
		Revoke::Renderer::Draw(m_Shader, m_VertexArray, objTrasnform);
			
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
	std::shared_ptr <Revoke::Shader> m_Shader;
	std::shared_ptr <Revoke::VertexArray> m_VertexArray;

	// Grid
	std::shared_ptr <Revoke::Shader> m_GridShader;
	std::shared_ptr <Revoke::VertexArray> m_GridVertexArray;

	Revoke::Camera m_Camera;
	glm::vec3 m_CameraPos;

	float m_Top = 0.9f, m_Down = -0.9f, m_Right = 1.6f, m_Left = -1.6f;

	float m_CamSpeed = 1.0f;
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