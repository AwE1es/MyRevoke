#pragma once
#include "MyRevoke.h"
#include "MyRevoke/Core/Core.h"

#include <filesystem>

namespace Revoke
{
	class ObjectsPannel
	{
	public:
		ObjectsPannel() = default;
		ObjectsPannel(Shared<Scene> currentScene);

		void SetScene(Shared<Scene> currentScene);
		void OnImGuiRender();

		void SceneHierarchyWindow(Entity entity);
		void PropertiesWindow();

		void SetSelectedEntity(Entity entity);
		Entity GetSelectedEntity() const { return m_SelectedEntity; }
	private:
		Shared<Scene> m_CurrentScene;
		Entity m_SelectedEntity;
	};
}