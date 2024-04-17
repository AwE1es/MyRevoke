#include "rvpch.h"
#include "SceneSettingsPannel.h"

#include <imgui.h>
#include <glm/gtc/type_ptr.hpp>

#include "MyRevoke/Renderer/RendererAPI.h"

namespace Revoke
{

	SceneSettingsPannel::SceneSettingsPannel()
	{
	}

	void SceneSettingsPannel::OnImGuiRender()
	{
		ImGui::Begin("Scene Settings");

		if (ImGui::DragFloat3("Set Clear Color", glm::value_ptr(m_ClearColor), 0.1f), 0.0f, 1.0f);
			RendererAPI::SetClearColor({ m_ClearColor.r, m_ClearColor.g, m_ClearColor.b, 1.0f });

		if(ImGui::Checkbox("Eneble Blending", &m_EnebleBlending))
			RendererAPI::EnableBlending();

		ImGui::End();
	}

}