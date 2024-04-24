#pragma once

#include <glm/glm.hpp>

#include "MyRevoke/Scene/Scene.h"

namespace Revoke
{
	
	class SceneSettingsPannel
	{
	public:
		SceneSettingsPannel();
		~SceneSettingsPannel();

		void OnImGuiRender();

		void SetScene(Shared<Scene> currentScene);

	private:
		int m_GravityPositionIteration = 4;
		int GravityVelocityIteration = 6;

		Shared<Scene> m_CurrentScene;
		glm::vec3 m_ClearColor = { 0.2, 0.2, 0.2 };
		bool m_EnebleBlending = true;
	};
}


