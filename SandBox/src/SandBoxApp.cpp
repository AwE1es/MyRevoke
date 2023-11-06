#include <MyRevoke.h>
#include "glm/glm.hpp"
#include "imgui.h"

class TestLayer : public Revoke::Layer
{
public:
	TestLayer()
		:Layer("Testing")
	{}
	void OnUpdate() override
	{
		if (Revoke::Input::IsKeyPressed(RV_KEY_TAB))
			RV_TRACE("TAB is pressed");
	}

	__declspec(dllimport) void OnImGuiDraw()
	{
		ImGui::Begin("Test");
		ImGui::Text("HI!");
		ImGui::End();

	}

	void OnEvent(Revoke::Event& e) override
	{
	
	}
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