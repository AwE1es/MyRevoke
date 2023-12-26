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
        auto group = m_Registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
        for (auto entity : group)
        {
            const auto& [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);

            Renderer2D::DrawQuad(transform, sprite.Color);
        }
    }
};
