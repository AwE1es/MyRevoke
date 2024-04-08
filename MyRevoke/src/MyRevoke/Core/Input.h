#pragma once

#include "Core.h"


namespace Revoke
{
	
	class Input
	{
	public:
		static bool IsKeyPressed(int keycode);
		static bool IsMousePressed(int mouseButton);

		static float GetMouseX();
		static float GetMouseY();

	};
}