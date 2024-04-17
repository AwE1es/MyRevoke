#pragma once

#include "Core.h"
#include <utility> 



namespace Revoke
{
	
	class Input
	{
	public:
		static bool IsKeyPressed(int key);
		static bool IsMousePressed(int mouseButton);

		static float GetMouseX();
		static float GetMouseY();

		static std::pair<float, float> GetMousePosition();
	};
}