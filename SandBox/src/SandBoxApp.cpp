#include <MyRevoke.h>
#include "MyRevoke/Core/EntryPoint.h"

#include "SandBox2D.h"


class SandBox : public Revoke::Application
{
public:
	SandBox()
	{
		PushLayer(new Renderer2DTest());
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