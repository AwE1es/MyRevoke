#include "rvpch.h"
#include "Application.h"
#include "Log.h"
#include "Input.h"

#include <glad/glad.h>



namespace Revoke
{
//#define BIND_EVENT_FUNC(x) std::bind(&Application::x, this, std::placeholders::_1)
	
	Application* Application::s_Instance = nullptr;
	



	Application::Application()
	{
		RV_CORE_ASSERT(!s_Instance, "Applicatio already exist");
		s_Instance = this;

		m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window->SetEventCallback(RV_BIND_EVENT_FUNK(Application::OnEvent));

		m_ImGuiLayer = new ImGuiLayer();
		PushOverlay(m_ImGuiLayer);

		m_VertexArray.reset(VertexArray::Create());


		//	Index Buffer
		std::shared_ptr<IndexBuffer> indexBuffer;
		float vertices[3 * 7] = {
			-0.5f, -0.5f, 0.0f, 1.0f, 0.3f, 0.1f, 1.0f,
			 0.5f, -0.5f, 0.0f,	0.2f, 1.0f, 0.1f, 1.0f,
			 0.0f,  0.5f, 0.0f,	0.2f, 0.3f, 1.0f, 1.0f
		};
		//	Vertex Buffer
		std::shared_ptr<VertexBuffer> vertexBuffer;
		vertexBuffer.reset(VertexBuffer::Create(vertices, sizeof(vertices)));

		BufferLayout layout = {
			{ShaderDataTypes::Float3, "Position", false},
			{ShaderDataTypes::Float4, "Color"}
		};
		vertexBuffer->SetLayout(layout);
		m_VertexArray->AddVertexBuffer(vertexBuffer);

		uint32_t indecies[3] = { 0, 1, 2 };
		indexBuffer.reset(IndexBuffer::Create(indecies, 3));
		m_VertexArray->SetIndexBuffer(indexBuffer);

		std::string vertexSrc = R"(
			
			#version 330 core
			
			layout(location = 0) in vec3 a_Pos;
			layout(location = 1) in vec4 a_Color;
			out vec3 v_Position;
			out vec4 v_Color;
	
			void main()
			{
				v_Position = a_Pos;
				v_Color = a_Color;
				gl_Position = vec4(a_Pos, 1.0);
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

		m_Shader.reset(new Shader(vertexSrc, fragmentSrc));
	}
	Application::~Application()
	{
	
	}
	void Application::Run()
	{
	
		while (m_Run)
		{
			glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);

			m_Shader->Bind();
			m_VertexArray->Bind();
			glDrawElements(GL_TRIANGLES, m_VertexArray->GetIndexBuffe()->GetCount(), GL_UNSIGNED_INT, nullptr);
			for (Layer* layer : m_LayerStack)
				layer->OnUpdate();

			m_ImGuiLayer->Begin();
			for (Layer* layer : m_LayerStack)
				layer->OnImGuiDraw();
			m_ImGuiLayer->End();


			m_Window->OnUpdate();
		}
	}
	void Application::OnEvent(Event& e)
	{

		EventDispatcher eDispatcher(e);
		
		eDispatcher.Dispatch<WindowsCloseEvent>(RV_BIND_EVENT_FUNK(Application::OnWindowsClose));

		

		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();)
		{
			(*--it)->OnEvent(e);
			if (e.Handled)
				break;
		}

	}
	void Application::PushLayer(Layer* layer)
	{
		m_LayerStack.PushLayer(layer);
	}
	void Application::PushOverlay(Layer* layer)
	{
		m_LayerStack.PushOverlay(layer);
	}
	bool Application::OnWindowsClose(WindowsCloseEvent e)
	{
		m_Run = false;
		return true;
	}
}