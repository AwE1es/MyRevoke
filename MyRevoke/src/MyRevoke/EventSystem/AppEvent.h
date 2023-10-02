#pragma once

#include "Event.h"

#include<sstream>

namespace Revoke {
	class RV_API WindowResizeEvent : public Event
	{
	public:
		WindowResizeEvent(unsigned int width, unsigned int height)
			: m_Width(width), m_Height(height) {}

		inline unsigned int GetWidth() const { return m_Width; }
		inline unsigned int GetHeight() const { return m_Height; }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "WindowResizeEvent: width = " << m_Width << ", height = " << m_Height;
			return ss.str();
		}

		static EventType GetStaticType() { return EventType::WindowResize; }
		virtual EventType GetEventType() const override { return GetStaticType(); }
		virtual const char* GetName() const override { return "WindowResize"; }
		virtual int GetCategoryFlags() const override { return EventCategoryApp; }

	private:
		unsigned int m_Width;
		unsigned int m_Height;
	};

	class RV_API WindowsCloseEvent : public Event
	{
	public:
		WindowsCloseEvent(){}
		static EventType GetStaticType() { return EventType::WindowClose; }
		virtual EventType GetEventType() const override { return GetStaticType(); }
		virtual const char* GetName() const override { return "WindowClose"; }
		virtual int GetCategoryFlags() const override { return EventCategoryApp; }
	};

}