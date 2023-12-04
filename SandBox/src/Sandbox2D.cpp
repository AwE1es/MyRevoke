#include "Sandbox2D.h"

#include <chrono>

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
void Renderer2DTest::OnUpdate(Revoke::Timestep deltaTime)
{
	PROFILE_SCOPE("Sandbox2D::OnUpdate");

	m_CameraController2D.OnUpdate(deltaTime);

	glm::mat4 objTrasnform = glm::translate(glm::mat4(1.0f), glm::vec3(m_MeshLocation, 0.0f));
	glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));

	Revoke::RenderCommand::ClearColor({ 0.2f, 0.2f, 0.2f, 1.0f });
	Revoke::RenderCommand::Clear();
	Revoke::RenderCommand::EnableBlending();

	Revoke::Renderer2D::Begin(m_CameraController2D.GetCamera());

	{
		PROFILE_SCOPE("Renderer Draw");
		Revoke::Renderer2D::DrawQuad(m_MeshLocation, m_MeshScale, m_Texture2D);
		Revoke::Renderer2D::DrawQuad({ 1.0f, 0.0f }, { 0.8f, 0.8f }, m_MeshColor);
		Revoke::Renderer2D::DrawTriangle({ 0.f, 0.0f }, { 0.8f, 0.8f }, m_MeshColor);
	}
	Revoke::Renderer2D::End();

}

	void Renderer2DTest::OnImGuiDraw()
	{
		ImGui::Begin("Settings");
		ImGui::ColorEdit4("Mesh Color: ", glm::value_ptr(m_MeshColor));
		ImGui::DragFloat2("Transforms: ", glm::value_ptr(m_MeshScale), 0.01f, 0.01f, 250.0f);
		ImGui::DragFloat2("Location: ", glm::value_ptr(m_MeshLocation), 0.01f);


		for (auto& result : m_ProfileResults)
		{
			char label[50];
			strcpy(label, "%.3fms ");
			strcat(label, result.Name);
			ImGui::Text(label, result.Time);
		}
		m_ProfileResults.clear();


		ImGui::End();
	}

	void Renderer2DTest::OnEvent(Revoke::Event& e) 
	{
		m_CameraController2D.OnEvent(e);
	}



	
