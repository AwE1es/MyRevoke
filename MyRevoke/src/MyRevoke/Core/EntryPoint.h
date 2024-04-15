#pragma once

extern Revoke::Application* Revoke::CreateApplication();

int main(int argc, char** argv)
{
	Revoke::Log::Init();
	auto application = Revoke::CreateApplication();
	application->Run();
	delete application;
}

