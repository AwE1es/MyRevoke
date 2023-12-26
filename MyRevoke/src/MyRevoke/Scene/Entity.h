#pragma once

#include "MyRevoke/Core/Core.h"
#include "Scene.h"

#include "entt.hpp"



namespace Revoke
{
	class Entity
	{
	public:
		Entity() = default;
		Entity(entt::entity handle, Scene* scene);
		Entity(const Entity& other) = default;

		template<typename T, typename... Args>
		T& AddComponent(Args&&... args)
		{
			RV_CORE_ASSERT(!HasComponent<T>(), "Component already exist in this Entity!");
			return m_Scene->m_Registry.emplace<T>(m_EntityHandle, std::forward<Args>(args)...);
		}
		template<typename T>
		T& GetComponent()
		{
			RV_CORE_ASSERT(HasComponent<T>(), "Component doesn't exist in this Entity!");
			return m_Scene->m_Registry.get<T>(m_EntityHandle);
		}
	
		template<typename T>
		void RemoveComponent()
		{
			RV_CORE_ASSERT(HasComponent<T>(), "Component doesn't exist in this Entity!");
			m_Scene->m_Registry.remove<T>(m_EntityHandle);
		}

		template<typename T>
		bool HasComponent()
		{
			return m_Scene->m_Registry.any_of<T>(m_EntityHandle);
		}

		operator bool() const { return m_EntityHandle != entt::null; }
	private:
		entt::entity m_EntityHandle = entt::null;
		Scene* m_Scene = nullptr;
	};
}