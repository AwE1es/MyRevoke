#pragma once

#include "Event.h"

#include<sstream>

namespace Revoke 
{

	
	class RV_API KeyEvent : public Event // abstructed class only for child clases (key pressed and key released)
	{
	public:
		inline int GetKeyCode() const { return m_KeyCode; }

		virtual int GetCategoryFlags() const override { return EventCategoryKeyboard | EventCategoryInput; }
	protected:
		KeyEvent(int keycode)
			:m_KeyCode(keycode) {}

		int m_KeyCode;
	};

	class RV_API KeyPressedEvent : public KeyEvent
	{
	public:
		KeyPressedEvent(int keycode, int repeatCount)
			: KeyEvent(keycode), m_RepeatCount(repeatCount) {}
		inline int GetRepeatCount() const { return m_RepeatCount; }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "KeyPresedEvent: " << m_KeyCode << " (" << m_RepeatCount << " repeats)";
			return ss.str();
		}

		static EventType GetStaticType() { return EventType::KeyPressed; } // used staic `couse keypressedevent alwways gonna be KeyPressed type
		virtual EventType GetEventType() const override { return GetStaticType(); }
		virtual const char* GetName() const override { return "KeyPressed"; }

	private:
		int m_RepeatCount;
	};

	class RV_API KeyReleasedEvent : public KeyEvent
	{
	public:
		KeyReleasedEvent(int keycode)
			: KeyEvent(keycode) {}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "KeyPresedEvent: " << m_KeyCode;
			return ss.str();
		}

		static EventType GetStaticType() { return EventType::KeyReleased; } 
		virtual EventType GetEventType() const override { return GetStaticType(); }
		virtual const char* GetName() const override { return "KeyReleased"; }

	private:
		int m_RepeatCount;
	};

}