#include<MyRevoke.h>

class TestLayer : public Revoke::Layer
{
public:
	TestLayer()
		:Layer("Testing")
	{}
	void OnUpdate() override
	{
		RV_INFO("Test layer is updated");
	}

	void OnEvent(Revoke::Event& e) override
	{
		RV_TRACE("{0}", e);
	}
};

class SandBox : public Revoke::Application
{
public:
	SandBox()
	{
		PushLayer(new TestLayer());
		PushOverlay(new Revoke::ImGuiLayer());
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