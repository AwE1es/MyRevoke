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

		void SetSelectedEntity(Entity entity);
		Entity GetSelectedEntity() const { return m_SelectedEntity; }
	private:
		Shared<Scene> m_CurrentScene;
		Entity m_SelectedEntity;
	};
}