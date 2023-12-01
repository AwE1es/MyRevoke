#include "Sandbox2D.h"


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

	m_CameraController2D.OnUpdate(deltaTime);

	glm::mat4 objTrasnform = glm::translate(glm::mat4(1.0f), glm::vec3(m_MeshLocation, 0.0f));
	glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));

	Revoke::RenderCommand::ClearColor({ 0.2f, 0.2f, 0.2f, 1.0f });
	Revoke::RenderCommand::Clear();
	Revoke::RenderCommand::EnableBlending();

	Revoke::Renderer2D::Begin(m_CameraController2D.GetCamera());

	Revoke::Renderer2D::DrawQuad(m_MeshLocation, m_MeshScale, m_Texture2D);
	Revoke::Renderer2D::DrawQuad({ 1.0f, 0.0f }, { 0.8f, 0.8f }, m_MeshColor);
	Revoke::Renderer2D::DrawTriangle({ 0.f, 0.0f }, { 0.8f, 0.8f }, m_MeshColor);
	
	Revoke::Renderer2D::End();

}

	void Renderer2DTest::OnImGuiDraw()
	{
		ImGui::Begin("Settings");
		ImGui::ColorEdit4("Mesh Color: ", glm::value_ptr(m_MeshColor));
		ImGui::DragFloat2("Transforms: ", glm::value_ptr(m_MeshScale), 0.01f, 0.01f, 250.0f);
		ImGui::DragFloat2("Location: ", glm::value_ptr(m_MeshLocation), 0.01f);
		ImGui::End();
	}

	void Renderer2DTest::OnEvent(Revoke::Event& e) 
	{
		m_CameraController2D.OnEvent(e);
	}
