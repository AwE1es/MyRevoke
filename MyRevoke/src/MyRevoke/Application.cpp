#include "Application.h"

#include "EventSystem/AppEvent.h"
#include "EventSystem/MouseEvent.h"
#include "Log.h"

namespace Revoke
{
	void Application::Run()
	{
		WindowResizeEvent e(1280, 720);
		MouseButtonPressedEvent s(1);
		RV_TRACE(s);
		RV_TRACE(e);
		while (true);
	}
}