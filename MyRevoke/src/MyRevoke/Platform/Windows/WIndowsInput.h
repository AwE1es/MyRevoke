#pragma once

#include "MyRevoke/Core/Core.h"
#include "MyRevoke/Core/Input.h"

namespace Revoke {
	class  WindowsInput : public Input
	{
	protected:
		virtual bool IsKeyPressedImplementation(int keycode) override;
		virtual bool IsMousePressedImplementation(int mouseButton) override;

		virtual float GetMouseXImplementation() override;
		virtual float GetMouseYImplementation() override;

	};
}

