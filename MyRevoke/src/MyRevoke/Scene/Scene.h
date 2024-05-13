#pragma once

#include "entt.hpp"

#include "MyRevoke/Core/Time.h"
#include "MyRevoke/Renderer/EditorCamera.h"
#include "MyRevoke/Core/UniversallyUniqueIdentifiers.h"

#include "MyRevoke/AudioManager/AudioSource.h"
#include "MyRevoke/AudioManager/AudioBuffer.h"


#include "MyRevoke/Scripting/NativeScript.h"

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
			Entity CreateEntity(UUID id, const std::string name = std::string());

			void OnRuntimeStart();

			void OnRuntimeUpdate(Timestep ts);
			void OnEditorUpdate(Timestep ts, EditorCamera& camera);

			void OnRuntimeStop();

			void OnViewportResize(uint32_t width, uint32_t height);

			void RemoveEntity(Entity ent);

			void SetName(std::string name) { m_Name = name; }
			std::string GetName() { return m_Name; }

			void SetGravityStats(int positionIteration, int velocityIteration) { SetGravityPositionIteration(positionIteration); SetGravityVelocityIteration(velocityIteration); }
			void SetGravityPositionIteration(int positionIteration) { m_PositionIteration = positionIteration; }
			void SetGravityVelocityIteration(int velocityIteration) { m_VelocityIteration = velocityIteration; }

			void OnSceneClose();

			Entity GetMainCamera() ;

	private:
		template<typename T>
		void OnComponentAdded(Entity entity, T& component);
	private:
		entt::registry m_Registry;
		uint32_t m_ViewportWidth = 0, m_ViewportHeight = 0;
		std::string m_Name = "Unnamed";

		b2World* m_B2World = nullptr;


		int m_PositionIteration = 4;
		int m_VelocityIteration = 6;

		friend class Entity;
		friend class ObjectsPannel;
		friend class Serealizer;
	};

}