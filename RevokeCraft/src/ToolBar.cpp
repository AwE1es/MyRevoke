#include "rvpch.h"

#include "ToolBar.h"
#include <imgui.h>

namespace Revoke
{
	ToolBar::ToolBar()
	{
		m_PlayIcon = std::make_shared<Texture>("resourses/icons/Toolbar/Play_Icon.png");
		m_StopIcon = std::make_shared<Texture>("resourses/icons/Toolbar/Stop_Icon.png");
	}
	void ToolBar::OnImGuiRender()
	{
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 2));
		ImGui::PushStyleVar(ImGuiStyleVar_ItemInnerSpacing, ImVec2(0, 0));
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.3f, 0.3f, 0.3f, 0.5f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.3f, 0.3f, 0.3f, 0.5f));
		ImGui::Begin("##toolbar",nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);

		float size = ImGui::GetWindowHeight() - 6.0f;
		ImGui::SameLine((ImGui::GetWindowContentRegionMax().x * 0.05f) - (size * 0.5f));
		Shared<Texture> icon = m_SceneState == SceneState::Editor ? m_PlayIcon : m_StopIcon;
		if (ImGui::ImageButton((ImTextureID)icon->GetID(), ImVec2(size,size), ImVec2(0, 0), ImVec2(1,1), 0))
		{
			if (m_SceneState == SceneState::Editor)
				OnScenePlay();
			else
				OnSceneStop();
		}

		ImGui::PopStyleVar(2);
		ImGui::PopStyleColor(3);
		ImGui::End();

	}
	void ToolBar::OnScenePlay()
	{	
		m_CurrentScene->OnRuntimeStart();
		m_SceneState = SceneState::Runtime;
	}
	void ToolBar::OnSceneStop()
	{
		m_CurrentScene->OnRuntimeStop();
		m_SceneState = SceneState::Editor;
	}
	void ToolBar::SetScene(Shared<Scene> currentScene)
	{
		m_CurrentScene = currentScene;
	}
}
