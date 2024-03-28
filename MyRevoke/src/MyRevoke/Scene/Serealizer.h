#pragma once

#include "Scene.h"


namespace Revoke {

	class Serealizer
	{
	public:
		Serealizer(const Shared<Scene> scene);
		void Serealize(const std::string& path);
		bool DeSerealize(const std::string& path);

	private:
		Shared<Scene> m_Scene;
	};

}
