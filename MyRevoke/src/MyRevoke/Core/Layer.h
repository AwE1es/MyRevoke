#pragma once

#include "Core.h"
#include "Time.h"

#include "MyRevoke/EventSystem/Event.h"

namespace Revoke {
	class  Layer
	{
	public:
		Layer(const std::string& name = "Layer");
		virtual ~Layer() = default;

		virtual void OnAttach() {}
		virtual void OnDetach() {}
		virtual void OnUpdate(Timestep deltaTime) {}
		virtual void OnImGuiDraw() {}
		virtual void OnEvent(Event& event) {}

	protected:
		std::string m_DebugName;
	};
}
