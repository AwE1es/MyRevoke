#include "ObjectsPannel.h"

#include "imgui.h"

#include <glm/gtc/type_ptr.hpp>

namespace Revoke
{
	extern const std::filesystem::path g_AssetsDirectory;

	ObjectsPannel::ObjectsPannel(Shared<Scene> currentScene)
		:m_CurrentScene(currentScene)
	{
	}

	void ObjectsPannel::SetScene(Shared<Scene> currentScene)
	{
		m_CurrentScene = currentScene;
		m_SelectedEntity = {};
	}

	void ObjectsPannel::OnImGuiRender()
	{
		ImGui::Begin("Scene Hierarchy");
		SceneHierarchyWindow();
		ImGui::End();

		ImGui::Begin("Properties");
		PropertiesWindow();
		ImGui::End();
	}
	void ObjectsPannel::SceneHierarchyWindow()
	{
		m_CurrentScene->m_Registry.each([&](auto entityID)
			{
				bool entityExist = true;
				Entity entity{ entityID , m_CurrentScene.get() };
				auto& entityName = entity.GetComponent<NameComponent>().Name;
				ImGuiTreeNodeFlags flags = ((m_SelectedEntity == entity) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
				bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)entity, flags, entityName.c_str());
				if (ImGui::IsItemClicked())
				{
					m_SelectedEntity = entity;
				}

				if (ImGui::BeginPopupContextItem(0, 1 | ImGuiPopupFlags_NoOpenOverItems))
				{
					if (ImGui::MenuItem("Delete Entity"))
					{
						entityExist = false;
					}
					ImGui::EndPopup();
				}

				if (opened)
				{
					ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow;
					bool opened = ImGui::TreeNodeEx((void*)9817239, flags, entityName.c_str());
					if (opened)
						ImGui::TreePop();
					ImGui::TreePop();
				}
				if (!entityExist)
				{
					m_CurrentScene->RemoveEntity(entity);
					if (m_SelectedEntity == entity)
						m_SelectedEntity = {};
				}
			});

		if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered())
			m_SelectedEntity = {};

		if (ImGui::BeginPopupContextWindow(0, 1 | ImGuiPopupFlags_NoOpenOverItems))
		{
			if (ImGui::MenuItem("Create Entity"))
			{
				m_CurrentScene->CreateEntity("Empty Entity");
			}
			ImGui::EndPopup();
		}
	}
	void ObjectsPannel::PropertiesWindow()
	{
		if (m_SelectedEntity)
		{
			if (m_SelectedEntity.HasComponent<NameComponent>())
			{
				auto& name = m_SelectedEntity.GetComponent<NameComponent>().Name;

				char buffer[256];
				memset(buffer, 0, sizeof(buffer));
				strcpy_s(buffer, 256, name.c_str());
				if (ImGui::InputText("Name", buffer, sizeof(buffer)))
				{
					name = std::string(buffer);
				}
			}

			if (m_SelectedEntity.HasComponent<TransformComponent>())
			{
				if (ImGui::TreeNodeEx((void*)typeid(TransformComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Transform"))
				{
					auto& transformComponent = m_SelectedEntity.GetComponent<TransformComponent>();

					ImGui::DragFloat3("Position", glm::value_ptr(transformComponent.Position), 0.1f);
					ImGui::DragFloat3("Rotation", glm::value_ptr(transformComponent.Rotation), 0.1f);
					ImGui::DragFloat3("Scale", glm::value_ptr(transformComponent.Scale), 0.1f);

					ImGui::TreePop();
				}
			}
			if (m_SelectedEntity.HasComponent<SpriteRendererComponent>())
			{
				if (ImGui::TreeNodeEx((void*)typeid(SpriteRendererComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Color"))
				{
					auto& spriteComponent = m_SelectedEntity.GetComponent<SpriteRendererComponent>();
					ImGui::ColorEdit4("Color", glm::value_ptr(spriteComponent.Color));

					ImGui::Button("Texture");

					if (ImGui::BeginDragDropTarget())
					{
						if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_PAYLOAD"))
						{
							const wchar_t* path = (const wchar_t*)payload->Data;
							std::filesystem::path texturePath = std::filesystem::path(g_AssetsDirectory) / path;

							spriteComponent.Texture2D = texturePath.string();
						}

						ImGui::EndDragDropTarget();
					}

					ImGui::TreePop();
				}
			}
			if (m_SelectedEntity.HasComponent<SoundComponent>())
			{
				if (ImGui::TreeNodeEx((void*)typeid(SoundComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Sound Settings"))
				{
					auto& soundComponent = m_SelectedEntity.GetComponent<SoundComponent>();

					ImGui::Button("File (.wov)");

					if (ImGui::BeginDragDropTarget())
					{
						if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_PAYLOAD"))
						{
							const wchar_t* path = (const wchar_t*)payload->Data;
							std::filesystem::path soundPath = std::filesystem::path(g_AssetsDirectory) / path;

							soundComponent.SetPath(soundPath.string());

						}
						ImGui::EndDragDropTarget();
					}
					ImGui::DragFloat("Pitch", &soundComponent.Pitch, 0.1f);
					ImGui::DragFloat("Gain", &soundComponent.Gain, 0.1f);
					ImGui::DragFloat3("Position", glm::value_ptr(soundComponent.Position), 0.1f);
					ImGui::DragFloat3("Velocity", glm::value_ptr(soundComponent.Velocity), 0.1f);

					ImGui::Checkbox("Loop Audio", &soundComponent.LoopSound);


					ImGui::TreePop();
				}
			}
			if (m_SelectedEntity.HasComponent<RigidBodyComponent>())
			{
				if (ImGui::TreeNodeEx((void*)typeid(RigidBodyComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Rigid Body"))
				{
					auto& bodyComponent = m_SelectedEntity.GetComponent<RigidBodyComponent>();

					const char* bodyType[] = { "StaticBody",  "KinematicBody", "DynamicBody" };
					const char* currentBodyType = bodyType[(int)bodyComponent.Type];

					if (ImGui::BeginCombo("Body Type", currentBodyType))
					{
						for (int i = 0; i < 3; i++)
						{
							bool isSelected = currentBodyType == bodyType[i];
							if (ImGui::Selectable(bodyType[i], isSelected))
							{
								currentBodyType = bodyType[i];
								bodyComponent.Type = (RigidBodyComponent::BodyType)i;
							}
							if (isSelected)
								ImGui::SetItemDefaultFocus();
						}

						ImGui::EndCombo();
					}
					ImGui::Checkbox("Fixed Rotation", &bodyComponent.IsRotating);

					ImGui::TreePop();
				}
			}

			if (m_SelectedEntity.HasComponent<BoxColisionComponent>())
			{
				if (ImGui::TreeNodeEx((void*)typeid(BoxColisionComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Colisions"))
				{
					auto& boxCollisionComponent = m_SelectedEntity.GetComponent<BoxColisionComponent>();

					ImGui::DragFloat2("Size", glm::value_ptr(boxCollisionComponent.Size), 0.1f);
					ImGui::DragFloat2("Offset", glm::value_ptr(boxCollisionComponent.Offset), 0.1f);

					ImGui::DragFloat("Density", &boxCollisionComponent.Density, 0.01f, 0.0f, 1.0f);
					ImGui::DragFloat("Friction", &boxCollisionComponent.Friction, 0.01f, 0.0f, 1.0f);
					ImGui::DragFloat("Restriction", &boxCollisionComponent.Restriction, 0.01f, 0.0f, 1.0f);
					ImGui::DragFloat("Resitution Treshhold", &boxCollisionComponent.ResitutionTreshhold, 0.01f, 0.0f);

					ImGui::Checkbox("Sensor", &boxCollisionComponent.isSensor);

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

			if (ImGui::Button("Add Component"))
				ImGui::OpenPopup("Add Component");

			//TODO add all components

			if (ImGui::BeginPopup("Add Component"))
			{
				if (ImGui::MenuItem("Sprite Renderer"))
				{
					m_SelectedEntity.AddComponent<SpriteRendererComponent>();
					ImGui::CloseCurrentPopup();
				}
				if (ImGui::MenuItem("Rigid Body"))
				{
					m_SelectedEntity.AddComponent<RigidBodyComponent>();
					ImGui::CloseCurrentPopup();
				}
				if (ImGui::MenuItem("Box Colidor"))
				{
					m_SelectedEntity.AddComponent<BoxColisionComponent>();
					ImGui::CloseCurrentPopup();
				}
				if (ImGui::MenuItem("Sound Component"))
				{
					m_SelectedEntity.AddComponent<SoundComponent>();
					ImGui::CloseCurrentPopup();
				}

				if (ImGui::MenuItem("Camera"))
				{
					m_SelectedEntity.AddComponent<CameraComponent>();
					ImGui::CloseCurrentPopup();
				}
				ImGui::EndPopup();
			}
		}
	}
	void ObjectsPannel::SetSelectedEntity(Entity entity)
	{
		m_SelectedEntity = entity;
	}
}
