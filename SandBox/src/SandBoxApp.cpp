#include<MyRevoke.h>

class SandBox : public Revoke::Application
{
public:
	SandBox()
	{

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