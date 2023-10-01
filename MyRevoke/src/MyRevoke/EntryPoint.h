#pragma once

#ifdef  RV_PLATFORM_WINDOWS

extern Revoke::Application* Revoke::CreateApplication();

int main(int argc, char** argv)
{
	Revoke::Log::Init();
	int a = 123;
	RV_CORE_ERROR("Error!! {0}", a);
	std::cout << "MyRevoke engine is running :)\n";
	auto app = Revoke::CreateApplication();
	app->Run();
	delete app;
}
#endif //  RV_PLATFORM_WINDOWS
