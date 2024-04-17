#pragma once

#include "MyRevoke/Renderer/Texture.h"
#include "MyRevoke/Core/Core.h"
#include "MyRevoke/Scene/Scene.h"

namespace Revoke
{
	enum class SceneState
	{
		Editor = 0,
		Runtime = 1
	};

	class ToolBar
	{
	public:
		ToolBar();
		void OnImGuiRender();

		void OnScenePlay();
		void OnSceneStop();

		void SetScene(Shared<Scene> currentScene);

		SceneState GetSceneState() const { return m_SceneState; }
	private:
	
		SceneState m_SceneState = SceneState::Editor;

		Shared<Texture> m_PlayIcon;
		Shared<Texture> m_StopIcon;

		Shared<Scene> m_CurrentScene;

	};
}


