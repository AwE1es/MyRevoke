#include "rvpch.h"

#include "MyRevoke/Renderer/Renderer2D.h"
#include "MyRevoke/AudioManager/AudioRenderer.h"

#include "Scene.h"
#include "Components.h"
#include "Entity.h"

#include <fstream>

#include <yaml-cpp/yaml.h>
#include <glm/glm.hpp>

#include <box2d/b2_world.h>
#include <box2d/b2_polygon_shape.h>
#include <box2d/b2_fixture.h>

#include <sndfile.h>

#include <AL/al.h>

//DELETE
#include "MyRevoke/Utility/KeyCodes.h"
#include "MyRevoke/Core/Input.h"



namespace Revoke
{

    Scene::Scene()
    {
    }

    Scene::Scene(std::string name )
        :m_Name(name)
    {     
    }

    Scene::~Scene()
    {
       
    }

    Entity Scene::CreateEntity(const std::string name)
    {
        Entity entity = { m_Registry.create(), this };
        entity.AddComponent<IdComponent>();
        entity.AddComponent<TransformComponent>();
        auto& entName = entity.AddComponent<NameComponent>();
        
        entName.Name = name.empty() ? "UnNamed Entity" : name;
        return entity;
    }

    Entity Scene::CreateEntity(UUID id, const std::string name)
    {
        Entity entity = { m_Registry.create(), this };
        entity.AddComponent<IdComponent>(id);
        entity.AddComponent<TransformComponent>();
        auto& entName = entity.AddComponent<NameComponent>();

        entName.Name = name.empty() ? "UnNamed Entity" : name;
        return entity;
    }

    void Scene::OnRuntimeStart()
    {
        m_ScriptEngine = std::make_shared<ScriptEngine>();


        b2Vec2 gravity = { 0.0f, -9.8f };
        m_B2World = new b2World(gravity);
        auto view = m_Registry.view<RigidBodyComponent>();
        for (auto ent : view)
        {
            Entity entity = { ent ,this };

            auto& rigitBody = entity.GetComponent<RigidBodyComponent>();
            auto& transforms = entity.GetComponent<TransformComponent>();

            b2BodyDef bodyDefenition;
            bodyDefenition.type = (b2BodyType)rigitBody.Type;
            bodyDefenition.position.Set(transforms.Position.x, transforms.Position.y);
            bodyDefenition.angle = transforms.Rotation.z;

            b2Body* body = m_B2World->CreateBody(&bodyDefenition);
            //TODO: add more options
            body->SetFixedRotation(rigitBody.IsRotating);

            rigitBody.Body = body;

            if (entity.HasComponent<BoxColisionComponent>())
            {
                auto& boxColidor = entity.GetComponent<BoxColisionComponent>();

                b2PolygonShape shape;
                shape.SetAsBox(transforms.Scale.x * boxColidor.Size.x, transforms.Scale.y * boxColidor.Size.y);

                b2FixtureDef fixture;
                fixture.shape = &shape;
                fixture.density = boxColidor.Density;
                fixture.friction = boxColidor.Friction;
                fixture.restitution = boxColidor.Restriction;
                fixture.restitutionThreshold = boxColidor.ResitutionTreshhold;
                fixture.isSensor = boxColidor.isSensor;

                //TODO: Save fixture?
                body->CreateFixture(&fixture);
            }
        }

        { // Sounds

            auto view = m_Registry.view<SoundComponent>();
            for (auto ent : view)
            {
                Entity entity = { ent, this };
                auto& soundComponent = entity.GetComponent<SoundComponent>();
                soundComponent.HasPlayed = false;

            };

        }
        
     
    }

    void Scene::OnRuntimeUpdate(Timestep ts)
    {
        m_ScriptEngine->OnUpdate();

        { // Scripts
            m_Registry.view<NativeScriptComponent>().each([=](auto entity, auto& nsc)
                {
                    if (!nsc.Instance)
                    {
                        nsc.Instance = m_ScriptEngine->GetScritpByName("Player");
                        nsc.Instance->m_Entity = Entity{ entity, this };

                        nsc.Instance->OnCreate();
                    }
                   
                   
                    nsc.Instance->OnUpdate(ts);
                    uint32_t t = (uint32_t)nsc.Instance->m_Entity.GetEnttId();
                });
        }

        {// Physics
            m_B2World->Step(ts, m_VelocityIteration, m_PositionIteration);

            auto view = m_Registry.view<RigidBodyComponent>();
            for (auto ent : view)
            {
                Entity entity = { ent, this };

                auto& rigitBody = entity.GetComponent<RigidBodyComponent>();
                auto& transforms = entity.GetComponent<TransformComponent>();

                b2Body* body = rigitBody.Body;
                const auto& pos = body->GetPosition();

                transforms.Position.x = pos.x;
                transforms.Position.y = pos.y;
                transforms.Rotation.z = body->GetAngle();
            };
        }
        

        { // Sounds

            auto view = m_Registry.view<SoundComponent>();
            for (auto ent : view)
            {
                Entity entity = { ent, this };
                auto& soundComponent = entity.GetComponent<SoundComponent>();

                if (!AudioRenderer::IsAudioActive(soundComponent))
                {
                    
                    if (!soundComponent.LoopSound && soundComponent.HasPlayed)
                        break;
              
                    AudioRenderer::RenderAudio(soundComponent);
                }

            };

        }
            

        { // Rendering

            Camera* mainCamera = nullptr;
            glm::mat4 cameraTransform;

            auto view = m_Registry.view<TransformComponent, CameraComponent>();

            for (auto entity : view)
            {
                auto [transform, camera] = view.get<TransformComponent, CameraComponent>(entity);

                if (camera.isMain)
                {
                    mainCamera = &camera.Camera;
                    cameraTransform = transform.GetTransform();
                    break;
                }
            }


            if (mainCamera)
            {
                Renderer2D::Begin(*mainCamera, cameraTransform);

                auto group = m_Registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
                for (auto entity : group)
                {
                    auto [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);
                    Renderer2D::DrawSprite(transform.GetTransform(), sprite, (int)entity);

                }
                Renderer2D::End();
            }

        }
      
    }
    void Scene::OnEditorUpdate(Timestep ts, EditorCamera& camera)
    {
        Renderer2D::Begin(camera);

        auto group = m_Registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
        for (auto entity : group)
        {
            auto [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);

            Renderer2D::DrawSprite(transform.GetTransform(), sprite, (int)entity);
           
      
        }
        Renderer2D::End();
    }
    void Scene::OnRuntimeStop()
    {

        delete m_B2World;
        m_B2World = nullptr;
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

    void Scene::OnSceneClose()
    {
        { // Sounds

            auto view = m_Registry.view<SoundComponent>();
            for (auto ent : view)
            {
                Entity entity = { ent, this };
                auto& soundComponent = entity.GetComponent<SoundComponent>();
                soundComponent.ShutDown();
            };

        }
    }

    Entity Scene::GetMainCamera()
    {
        auto view = m_Registry.view<CameraComponent>();
        for (auto ent : view)
        {
            const auto& camera = view.get<CameraComponent>(ent);
            if (camera.isMain)
            {
               
                return Entity{ ent, this };
            }
        }
        return {};
    }



    template<typename T>
    void Scene::OnComponentAdded(Entity entity, T& component)
    {
       
    }
    template<>
    void Scene::OnComponentAdded<IdComponent>(Entity entity, IdComponent& component)
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
    template<>
    void Scene::OnComponentAdded<RigidBodyComponent>(Entity entity, RigidBodyComponent& component)
    {

    }
    template<>
    void Scene::OnComponentAdded<BoxColisionComponent>(Entity entity, BoxColisionComponent& component)
    {

    }
    template<>
    void Scene::OnComponentAdded<SoundComponent>(Entity entity, SoundComponent& component)
    {

    }

    template void Scene::OnComponentAdded(Entity, IdComponent&);
    template void Scene::OnComponentAdded(Entity, TransformComponent&);
    template void Scene::OnComponentAdded(Entity, CameraComponent&);
    template void Scene::OnComponentAdded(Entity, SpriteRendererComponent&);
    template void Scene::OnComponentAdded(Entity, NameComponent&);
    template void Scene::OnComponentAdded(Entity, NativeScriptComponent&);
};

