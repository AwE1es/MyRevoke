#pragma once
#include "MyRevoke.h"

namespace Revoke
{
	class ObjectsPannel
	{
	public:
		ObjectsPannel() = default;
		ObjectsPannel(Shared<Scene> currentScene);

		void SetScene(Shared<Scene> currentScene);
		void OnImGuiRender();
	private:
		Shared<Scene> m_CurrentScene;
		Entity m_SelectedEntity;
	};
}