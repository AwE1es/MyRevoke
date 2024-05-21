#pragma once

extern Revoke::Application* Revoke::CreateApplication();

int main()
{
	Revoke::Log::Init();
	Revoke::Application* application = Revoke::CreateApplication();
	application->Run();
	delete application;
}

