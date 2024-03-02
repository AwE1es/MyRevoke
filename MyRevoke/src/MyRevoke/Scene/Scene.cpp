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
        auto& entName = entity.AddComponent<NameComponent>();
        
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
        glm::mat4 cameraTransform;

        auto view = m_Registry.view<TransformComponent, CameraComponent>();

        for (auto entity : view)
        {
            const auto [transform, camera] = view.get<TransformComponent, CameraComponent>(entity);

            if (camera.isMain)
            {
                mainCamera = &camera.Camera;
                cameraTransform = transform.GetTransform();
                break;
            }
        }

        if (mainCamera)
            Renderer2D::Begin(mainCamera->GetProjectionMatrix(), cameraTransform);

        auto group1 = m_Registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
        for (auto entity : group1)
        {
            auto [transform, sprite] = group1.get<TransformComponent, SpriteRendererComponent>(entity);

            if (sprite.TexturePath == "")
            {
                Renderer2D::DrawQuad(transform.GetTransform(), sprite.Color);
            }
            else
            {
                Shared <Texture2D> texture;
                texture = Texture2D::Create(sprite.TexturePath);
                Renderer2D::DrawQuad(transform.GetTransform(), texture);
            }
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
    void Scene::RemoveEntity(Entity ent)
    {
       m_Registry.destroy(ent);
    }

    
    template<typename T>
    void Scene::OnComponentAdded(Entity entity, T& component)
    {
       
    }

    template<>
    void Scene::OnComponentAdded<TransformComponent>(Entity entity, TransformComponent& component)
    {

    }
    template<>
    void Scene::OnComponentAdded<CameraComponent>(Entity entity, CameraComponent& component)
    {
        component.Camera.SetViewportSize(m_ViewportWidth, m_ViewportHeight);
    }
    template<>
    void Scene::OnComponentAdded<SpriteRendererComponent>(Entity entity, SpriteRendererComponent& component)
    {

    }
    template<>
    void Scene::OnComponentAdded<NameComponent>(Entity entity, NameComponent& component)
    {

    }
    template<>
    void Scene::OnComponentAdded<NativeScriptComponent>(Entity entity, NativeScriptComponent& component)
    {

    }
    
    template void Scene::OnComponentAdded(Entity, TransformComponent&);
    template void Scene::OnComponentAdded(Entity, CameraComponent&);
    template void Scene::OnComponentAdded(Entity, SpriteRendererComponent&);
    template void Scene::OnComponentAdded(Entity, NameComponent&);
    template void Scene::OnComponentAdded(Entity, NativeScriptComponent&);
};

