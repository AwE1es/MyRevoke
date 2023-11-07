#pragma once

#include "MyRevoke/Core.h"


namespace Revoke
{
	class  Input
	{
	public:
		inline static bool IsKeyPressed(int keycode) { return s_Intance->IsKeyPressedImplementation(keycode); }
		inline static bool IsMousePressed(int mouseButton) { return s_Intance->IsMousePressedImplementation(mouseButton); }

		inline static float GetMouseX() { return s_Intance->GetMouseXImplementation(); }
		inline static float GetMouseY() { return s_Intance->GetMouseYImplementation(); }
	protected:
		virtual bool IsKeyPressedImplementation(int keycde) = 0;
		virtual bool IsMousePressedImplementation(int keycde) = 0;

		virtual float GetMouseXImplementation() = 0;
		virtual float GetMouseYImplementation() = 0;
	private:
		static Input* s_Intance;
	};


}