#pragma once

#include "rvpch.h"

#include "MyRevoke/Core.h"
#include "MyRevoke/EventSystem/Event.h"


namespace Revoke {

	struct WindowSettings
	{
		std::string Title;
		unsigned int Width;
		unsigned int Height;

		WindowSettings(const std::string& title = "MyRevoke",
			unsigned int width = 1280,
			unsigned int height = 720)
			: Title(title), Width(width), Height(height){}
		                               
	};

	class RV_API Window // Inteface that going to be interprited by platform
	{
	public:
		virtual ~Window() {}
		virtual void OnUpdate() = 0;

		virtual unsigned int GetWidth() const = 0;
		virtual unsigned int GetHeight() const = 0;

		//Atributes

		virtual void SetEventCallback(const std::function<void(Event&)>& callback) = 0;
		virtual void SetVSync(bool enabled) = 0;
		virtual bool IsVSync() const = 0; 

		static Window* Create(const WindowSettings& props = WindowSettings());

	};
}

