#pragma once

#ifdef  RV_PLATFORM_WINDOWS

extern Revoke::Application* Revoke::CreateApplication();

int main(int argc, char** argv)
{
	std::cout << "MyRevoke engine is running :)\n";
	auto app = Revoke::CreateApplication();
	app->Run();
	delete app;
}
#endif //  RV_PLATFORM_WINDOWS
