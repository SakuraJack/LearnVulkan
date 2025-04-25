#pragma once

enum class EventType
{
	None = 0,
	WindowClose, WindowResize, WindowFocus, WindowLostFocus, WindowMoved
};

#define EVENT_CLASS_TYPE(type) static EventType GetStaticType() { return EventType::##type; }\
								virtual EventType GetType() const override { return GetStaticType(); }\
								virtual const char* GetName() const override { return #type; }	

class Event
{
public:
	virtual ~Event() = default;
	virtual EventType GetType() const = 0;
	virtual const char* GetName() const = 0;
	virtual std::string ToString() const { return GetName(); }

	bool Handled = false;
};

class EventDispatcher
{
	template<typename T>
	using EventFn = std::function<bool(T&)>;

public:
	EventDispatcher(Event& event)
		: m_Event(event)
	{}

	template<typename T>
	bool Dispatch(EventFn<T> func)
	{
		if (m_Event.GetType() == T::GetStaticType() && !m_Event.Handled)
		{
			m_Event.Handled = func(*(T*)&m_Event);
			return true;
		}
		return false;
	}

private:
	Event& m_Event;
};