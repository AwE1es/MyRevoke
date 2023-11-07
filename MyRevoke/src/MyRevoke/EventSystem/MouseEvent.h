#pragma once

#include "Event.h"

#include "rvpch.h"

namespace Revoke
{

	class  MouseMovedEvent: public Event
	{
	public:
		
		MouseMovedEvent(float x, float y)
			: m_MoveX(x), m_MoveY(y) {}

		inline float GetX() const { return m_MoveX; }
		inline float GetY() const { return m_MoveY; }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MouseMovedEvent: x = " << m_MoveX << ", y = " << m_MoveY;
			return ss.str();
		}

		static EventType GetStaticType() { return EventType::MouseMoved; } 
		virtual EventType GetEventType() const override { return GetStaticType(); }
		virtual const char* GetName() const override { return "MouseMoved"; }
		virtual int GetCategoryFlags() const override { return EventCategoryMouse | EventCategoryInput; }
	private:
		float m_MoveX, m_MoveY;
	};

	class  MouseButtonEvent : public Event
	{
	public:
		inline int GetMouseButton() const { return m_Button; }

		virtual int GetCategoryFlags() const override { return EventCategoryMouse | EventCategoryInput; }
	protected:
		MouseButtonEvent(int button)
			: m_Button(button) {}
		int m_Button;
	};

	class  MouseButtonPressedEvent : public MouseButtonEvent
	{
	public:
		MouseButtonPressedEvent( int presedButton)
		: MouseButtonEvent(presedButton) {}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MouseButtonPressedEvent: " << m_Button;
			return ss.str();
		}

		static EventType GetStaticType() { return EventType::MouseButtonPressed; } 
		virtual EventType GetEventType() const override { return GetStaticType(); }
		virtual const char* GetName() const override { return "MouseButtonPressed"; }
	};

	class  MouseButtonReleasedEvent : public MouseButtonEvent
	{
	public:
		MouseButtonReleasedEvent(int releasedButton)
			: MouseButtonEvent(releasedButton) {}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MouseButtonReleasedEvent: " << m_Button;
			return ss.str();
		}

		static EventType GetStaticType() { return EventType::MouseButtonReleased; }
		virtual EventType GetEventType() const override { return GetStaticType(); }
		virtual const char* GetName() const override { return "MouseButtonReleased"; }
	};
	class  MouseScrolledEvent : public Event
	{
	public:
		MouseScrolledEvent(int deltaX, int deltaY)
			: m_DeltaX(deltaX), m_DeltaY(deltaY) {}

		inline int GetDeltaX() const { return m_DeltaX; }
		inline int GetDeltaY() const { return m_DeltaY; }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MouseScrolledEvent : " << GetDeltaX() << ", " << GetDeltaY();
			return ss.str();
		}

		static EventType GetStaticType() { return EventType::MouseScrolled; }
		virtual EventType GetEventType() const override { return GetStaticType(); }
		virtual const char* GetName() const override { return "MouseScrolled"; }
		virtual int GetCategoryFlags() const override { return EventCategoryMouse | EventCategoryInput; }

	private:
		int m_DeltaX, m_DeltaY;
	};

}