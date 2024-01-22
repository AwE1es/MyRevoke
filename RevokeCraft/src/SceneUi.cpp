#include "SceneUi.h"

#include "imgui.h"
#include <glm/gtc/type_ptr.hpp>

namespace Revoke
{

	ObjectsPannel::ObjectsPannel(Shared<Scene> currentScene)
		:m_CurrentScene(currentScene)
	{
	}

	void ObjectsPannel::SetScene(Shared<Scene> currentScene)
	{
		m_CurrentScene = currentScene;
	}

	void ObjectsPannel::OnImGuiRender()
	{
		ImGui::Begin("Scene Hierarchy");

		m_CurrentScene->m_Registry.each([&](auto entityID)
			{
				Entity entity{ entityID , m_CurrentScene.get() };
				auto& entityName = entity.GetComponent<NameComponent>().Name;
				ImGuiTreeNodeFlags flags = ((m_SelectedEntity == entity) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
				bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)entity, flags, entityName.c_str());
				if (ImGui::IsItemClicked())
				{
					m_SelectedEntity = entity;
				}

				if (opened)
				{
					ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow;
					bool opened = ImGui::TreeNodeEx((void*)9817239, flags, entityName.c_str());
					if (opened)
						ImGui::TreePop();
					ImGui::TreePop();
				}
			});

		if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered())
			m_SelectedEntity = {};

		ImGui::End();

		ImGui::Begin("Properties");
		if (m_SelectedEntity)
		{
			if (m_SelectedEntity.HasComponent<NameComponent>())
			{
				auto& tag = m_SelectedEntity.GetComponent<NameComponent>().Name;

				char buffer[256];
				memset(buffer, 0, sizeof(buffer));
				strcpy_s(buffer, 256, tag.c_str());
				if (ImGui::InputText("Name", buffer, sizeof(buffer)))
				{
					tag = std::string(buffer);
				}

			}
			if (m_SelectedEntity.HasComponent<TransformComponent>())
			{
				if (ImGui::TreeNodeEx((void*)typeid(TransformComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Transform"))
				{
					auto& transform = m_SelectedEntity.GetComponent<TransformComponent>().Transform;
					ImGui::DragFloat3("Position", glm::value_ptr(transform[3]), 0.1f);

					ImGui::TreePop();
				}
			}

			if (m_SelectedEntity.HasComponent<CameraComponent>())
			{
				if (ImGui::TreeNodeEx((void*)typeid(CameraComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Camera"))
				{
					auto& cameraComponent = m_SelectedEntity.GetComponent<CameraComponent>();

					ImGui::Checkbox("Primary", &cameraComponent.isMain);


					const char* projectionType[] = { "3D Camera", "2D Camera" };
					const char* currentProjectionType = projectionType[(int)cameraComponent.Camera.GetProjectionType()];

					if (ImGui::BeginCombo("Camera Type", currentProjectionType))
					{
						for (int i = 0; i < 2; i++)
						{
							bool isSelected = currentProjectionType == projectionType[i];
							if (ImGui::Selectable(projectionType[i], isSelected))
							{
								currentProjectionType = projectionType[i];
								cameraComponent.Camera.SetProjectionType((SceneCamera::Projection)i);
							}
							if (isSelected)
								ImGui::SetItemDefaultFocus();
						}

						ImGui::EndCombo();
					}

					if (cameraComponent.Camera.GetProjectionType() == SceneCamera::Projection::Perspective)
					{
						float FOV = glm::degrees(cameraComponent.Camera.GetPerspFOV());
						if (ImGui::DragFloat("FOV", &FOV))
							cameraComponent.Camera.SetPerspFOV(glm::radians(FOV));

						float nearClip = cameraComponent.Camera.GetPerspNearClip();
						if (ImGui::DragFloat("Near Clip", &nearClip))
							cameraComponent.Camera.SetPerspNearClip(nearClip);

						float farClip = cameraComponent.Camera.GetPerspFarClip();
						if (ImGui::DragFloat("Far Clip", &farClip))
							cameraComponent.Camera.SetPerspFarClipl(farClip);
					}
					if (cameraComponent.Camera.GetProjectionType() == SceneCamera::Projection::Orthographic)
					{
						float size = cameraComponent.Camera.GetOrthoSize();
						if (ImGui::DragFloat("Size", &size))
							cameraComponent.Camera.SetOrthoSize(size);

						float nearClip = cameraComponent.Camera.GetOrthoNearClip();
						if (ImGui::DragFloat("Near Clip", &nearClip))
							cameraComponent.Camera.SetOrthoNearClip(nearClip);

						float farClip = cameraComponent.Camera.GetOrthoFarClip();
						if (ImGui::DragFloat("Far Clip", &farClip))
							cameraComponent.Camera.SetOrthoFarClipl(farClip);

					}
					ImGui::TreePop();
				}
			}
		}
		ImGui::End();
	}
}
