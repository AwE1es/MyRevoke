#include "rvpch.h"
#include "Entity.h"

Revoke::Entity::Entity(entt::entity handle, Scene* scene)
	:m_EntityHandle(handle), m_Scene(scene) {}

