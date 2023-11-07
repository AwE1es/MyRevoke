#pragma once

#include "MyRevoke/Layer.h"

#include "MyRevoke/EventSystem/MouseEvent.h"
#include "MyRevoke/EventSystem/KeyEvent.h"
#include "MyRevoke/EventSystem/AppEvent.h"

namespace Revoke {

	class  ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer();

		void OnAttach() override;
		void OnDetach() override;
		void OnImGuiDraw() override;

		void Begin();
		void End();
	private:

	private:
		float m_Time = 0.0f;
	};

}