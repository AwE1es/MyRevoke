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
			if(m_EntityHandle != entt::null)
				return m_Scene->m_Registry.any_of<T>(m_EntityHandle);
			return false;
		}
		entt::entity GetEnttId() const { return m_EntityHandle; }
		
		operator entt::entity() const { return m_EntityHandle; }
		operator uint32_t() const { return (uint32_t)m_EntityHandle; }
		operator bool() const { return m_EntityHandle != entt::null; }

		bool operator==(const Entity& other) const
		{
			return m_EntityHandle == other.m_EntityHandle && m_Scene == other.m_Scene;
		}

		bool operator!=(const Entity& other) const
		{
			return !(*this == other);
		}
	private:
		entt::entity m_EntityHandle = entt::null;
		Scene* m_Scene = nullptr;
	};
}