#pragma once

#include "MyRevoke/Core/Core.h"

#include "rvpch.h"

// Event system
namespace Revoke
{
	enum class EventType															// Used to give indicator for all evenst
	{
		None = 0,
		WindowClose, WindowResize, WindowMoved,										// Covered in AppEvent.h
		KeyPressed, KeyReleased, KeyTyped,											// Covered in KeyEvent.h
		MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled			// Covered in MouseEvnt.h
	};

	enum EventCategory																// Used to give indicator to specific group of events
	{
		None						= 0,
		EventCategoryApp			= BIT(0),
		EventCategoryInput			= BIT(1),
		EventCategoryKeyboard		= BIT(2),
		EventCategoryMouse			= BIT(3),
		EventCategoryMouseButton	= BIT(4)

	};
	class  Event //abstract base class for all the events
	{
		friend class EventDispatcher;
	public:
		virtual EventType GetEventType() const = 0;
		virtual const char* GetName() const = 0;
		virtual int GetCategoryFlags() const = 0;
		virtual std::string ToString() const { return GetName(); }

		inline bool IsCategory(EventCategory category)
		{
			return GetCategoryFlags() & category;
		}
		bool IsHandeled() const { return Handled; }
	public:
		bool Handled = false;
	protected:
	};

	class EventDispatcher
	{
	public:
		EventDispatcher(Event& event)
			:m_Event(event) {}

		template<typename T>
		bool Dispatch(std::function <bool(T&)> func)
		{
			if (m_Event.GetEventType() == T::GetStaticType())
			{
				m_Event.Handled = func(*(T*)&m_Event);
				return true;
			}
			return false;
		}

	private:
		Event& m_Event;
	};

	inline std::ostream& operator<<(std::ostream& os, const Event& e)
	{
		return os << e.ToString();
	}
}