#include "rvpch.h"

#include "MyRevoke/Renderer/Renderer2D.h"
#include "Scene.h"
#include "Components.h"
#include "Entity.h"

#include <glm/glm.hpp>

namespace Revoke
{

    Scene::Scene()
    {

    }

    Scene::~Scene()
    {
    }

    Entity Scene::CreateEntity(const std::string name)
    {
        Entity entity = { m_Registry.create(), this };
        entity.AddComponent<TransformComponent>();
        auto& entName = entity.AddComponent<NameCpmponent>();
        
        entName.Name = name.empty() ? "UnNamed Entity" : name;
        return entity;
    }

    void Scene::OnUpdate(Timestep ts)
    {

        {
            m_Registry.view<NativeScriptComponent>().each([=](auto entity, auto& nsc)
                {
                    if (!nsc.Instance)
                    {
                        nsc.Instance = nsc.InstantiateScript();
                        nsc.Instance->m_Entity = Entity{ entity, this };

                        nsc.Instance->OnCreate();
                    }

                    nsc.Instance->OnUpdate(ts);
                });
        }


        Camera* mainCamera = nullptr;
        glm::mat4* cameraTransform = nullptr;

        auto view = m_Registry.view<TransformComponent, CameraComponent>();

        for (auto entity : view)
        {
            const auto [transform, camera] = view.get<TransformComponent, CameraComponent>(entity);

            if (camera.isMain)
            {
                mainCamera = &camera.Camera;
                cameraTransform = &transform.Transform;
                break;
            }
        }

        if (mainCamera)
            Renderer2D::Begin(mainCamera->GetProjectionMatrix(), *cameraTransform);

        auto group1 = m_Registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
        for (auto entity : group1)
        {
            auto [transform, sprite] = group1.get<TransformComponent, SpriteRendererComponent>(entity);

            Renderer2D::DrawQuad(transform, sprite.Color);
        }

        Renderer2D::End();
    }
    void Scene::OnViewportResize(uint32_t width, uint32_t height)
    {
        m_ViewportWidth = width;
        m_ViewportHeight = height;

        auto view = m_Registry.view<CameraComponent>();
        for (auto entity : view)
        {
            auto& cameraComponent = view.get<CameraComponent>(entity);
            if (!cameraComponent.FixedAspectRatio)
                cameraComponent.Camera.SetViewportSize(width, height);
        }

    }
};
