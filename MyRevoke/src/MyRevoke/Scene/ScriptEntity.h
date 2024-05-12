#pragma once
#include "Entity.h"

namespace Revoke
{
	class ScriptEntity
	{
	public:
		virtual ~ScriptEntity() {}

		template<typename T>
		T& GetComponent()
		{
			return m_Entity.GetComponent<T>();
		}
		virtual void OnCreate() {}
	protected:
		virtual void OnDestroy() {}
		virtual void OnUpdate(Timestep ts) {}
	private:
		Entity m_Entity;
		friend class Scene;
	};
}