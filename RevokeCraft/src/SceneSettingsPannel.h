#pragma once

#include <glm/glm.hpp>
namespace Revoke
{
	
	class SceneSettingsPannel
	{
	public:
		SceneSettingsPannel();
		void OnImGuiRender();
	private:
		glm::vec3 m_ClearColor = { 0.2, 0.2, 0.2 };
		bool m_EnebleBlending = true;
	};
}


