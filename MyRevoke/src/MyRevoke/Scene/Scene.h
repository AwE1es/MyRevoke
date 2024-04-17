#pragma once

#include "entt.hpp"

#include "MyRevoke/Core/Time.h"
#include "MyRevoke/Renderer/EditorCamera.h"

class b2World;

namespace Revoke
{
	class Entity;


	class Scene
	{
		public:
			Scene();
			Scene(std::string name);
			~Scene();
			
			Entity CreateEntity(const std::string name = std::string());

			void OnRuntimeStart();

			void OnRuntimeUpdate(Timestep ts);
			void OnEditorUpdate(Timestep ts, EditorCamera& camera);

			void OnRuntimeStop();

			void OnViewportResize(uint32_t width, uint32_t height);

			void RemoveEntity(Entity ent);

			void SetName(std::string name) { m_Name = name; }
			std::string GetName() { return m_Name; }

			Entity GetMainCamera() ;
			
	private:
		template<typename T>
		void OnComponentAdded(Entity entity, T& component);
	private:
		entt::registry m_Registry;
		uint32_t m_ViewportWidth = 0, m_ViewportHeight = 0;
		std::string m_Name;

		b2World* m_B2World = nullptr;

		friend class Entity;
		friend class ObjectsPannel;
		friend class Serealizer;
	};

}