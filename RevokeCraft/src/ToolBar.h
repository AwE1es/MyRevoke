#pragma once

#include "MyRevoke/Renderer/Texture.h"
#include "MyRevoke/Core/Core.h"

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
		SceneState GetSceneState() const { return m_SceneState; }
	private:
	

		SceneState m_SceneState = SceneState::Editor;

		Shared<Texture2D> m_PlayIcon;
		Shared<Texture2D> m_StopIcon;

	};
}


