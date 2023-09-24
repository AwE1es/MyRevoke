#pragma once

#include"Core.h"

namespace Revoke
{
	class RV_API Application
	{
	public:
		Application() {}
		virtual ~Application() {}

		void Run();

	};

	//This function will be defined by a client
	Application* CreateApplication();
}
