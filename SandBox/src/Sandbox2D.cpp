#include "Sandbox2D.h"

#include <chrono>
#include <string>


template<typename Fn>
class Timer
{
public:
	Timer(const char* name, Fn&& func)
		: m_Name(name), m_Func(func), m_Stopped(false)
	{
		m_StartTimepoint = std::chrono::high_resolution_clock::now();
	}

	~Timer()
	{
		if (!m_Stopped)
			Stop();
	}

	void Stop()
	{
		auto endTimepoint = std::chrono::high_resolution_clock::now();

		long long start = std::chrono::time_point_cast<std::chrono::microseconds>(m_StartTimepoint).time_since_epoch().count();
		long long end = std::chrono::time_point_cast<std::chrono::microseconds>(endTimepoint).time_since_epoch().count();

		m_Stopped = true;

		float duration = (end - start) * 0.001f;
		m_Func({ m_Name, duration });
	}
private:
	const char* m_Name;
	Fn m_Func;
	std::chrono::time_point<std::chrono::steady_clock> m_StartTimepoint;
	bool m_Stopped;
};

#define PROFILE_SCOPE(name) Timer timer##__LINE__(name, [&](ProfileResult profileResult) { m_ProfileResults.push_back(profileResult); })


Renderer2DTest::Renderer2DTest()
		:Layer("Testing"), m_CameraController2D(1280.0f / 720.0f)
	{
		m_MeshColor = { 1.0f, 1.0f, 0.0f, 1.0f };
		m_MeshLocation = { -1.0f, 0.0f };
		m_MeshScale = { 0.8f, 0.8f };
	}
void Renderer2DTest::OnAttach()
{
	m_Texture2D = Revoke::Texture2D::Create("Textures/Triangle_Texture.png");
}
void Renderer2DTest::OnDetach() 
{

}
static int numberOfQuads = 0;
void Renderer2DTest::OnUpdate(Revoke::Timestep deltaTime)
{
	PROFILE_SCOPE("Sandbox2D::OnUpdate");
	Revoke::Renderer2D::ResetStatistics();
	m_CameraController2D.OnUpdate(deltaTime);

	Revoke::RenderCommand::ClearColor({ 0.2f, 0.2f, 0.2f, 1.0f });
	Revoke::RenderCommand::Clear();
	Revoke::RenderCommand::EnableBlending();

	Revoke::Renderer2D::Begin(m_CameraController2D.GetCamera());

	{
		PROFILE_SCOPE("Renderer Draw");
		Revoke::Renderer2D::DrawQuad(m_MeshLocation, m_MeshScale, m_Texture2D);
		for (int i = -100; i < 100; i++) 
		{
			for (int j = -100; j < 100; j++)
			{
				Revoke::Renderer2D::DrawQuad({ i, j}, { 0.8f, 0.8f }, { 0.0f, 0.4f, 0.5f, 1.0f });
			}
		}
		Revoke::Renderer2D::DrawQuad(m_MeshLocation, m_MeshScale, m_Texture2D);
		//Revoke::Renderer2D::DrawTriangle({ 0.f, 0.0f }, { 0.8f, 0.8f }, m_MeshColor);
	}
	Revoke::Renderer2D::End();

}

	void Renderer2DTest::OnImGuiDraw()
	{
		ImGui::Begin("Settings");
		ImGui::ColorEdit4("Mesh Color: ", glm::value_ptr(m_MeshColor));
		ImGui::DragFloat2("Transforms: ", glm::value_ptr(m_MeshScale), 0.01f, 0.01f, 250.0f);
		ImGui::DragFloat2("Location: ", glm::value_ptr(m_MeshLocation), 0.01f);

		auto stats = Revoke::Renderer2D::GetStats();
		ImGui::Text("Renderer Stats");
		ImGui::Text("Draw Calls: %d", stats.DrawCalls);
		ImGui::Text("Quad Count: %d", stats.QuadCount);
		ImGui::Text("Index Count: %d", stats.GetTotalIndexCount());
		ImGui::Text("Vertex Count: %d", stats.GetTotalVertexCount());
		ImGui::Text("---------------------");
		float totalFrameTime = 0.0f;
		float displayUpdateInterval = 0.25f;  

		for (auto& result : m_ProfileResults)
		{
			totalFrameTime += result.Time;
			char label[50];
			sprintf(label, "%.3fms %s", result.Time, result.Name);
			ImGui::Text(label);
		}

		float averageFrameTime = totalFrameTime / m_ProfileResults.size();
		float frameRate = 1000.0f / averageFrameTime;

		static float timeSinceLastUpdate = 0.0f;
		timeSinceLastUpdate += ImGui::GetIO().DeltaTime;
		
		if (timeSinceLastUpdate >= displayUpdateInterval)
		{
			m_Framerate = frameRate;
			timeSinceLastUpdate = 0.0f;
		}
		char frameRateLabel[50];
		sprintf(frameRateLabel, "FPS: %.2f FPS", m_Framerate);
		ImGui::Text(frameRateLabel);

	

		m_ProfileResults.clear();

		ImGui::End();
		numberOfQuads = 0;
	}

	void Renderer2DTest::OnEvent(Revoke::Event& e) 
	{
		m_CameraController2D.OnEvent(e);
	}



	
