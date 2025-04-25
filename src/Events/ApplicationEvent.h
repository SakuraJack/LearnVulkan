#pragma once
#include "Event.h"

class WindowCloseEvent : public Event
{
public:
	WindowCloseEvent() {}

	EVENT_CLASS_TYPE(WindowClose)
};

class WindowResizeEvent : public Event
{
public:
	WindowResizeEvent(int width, int height)
		: m_Width(width), m_Height(height) {}

	EVENT_CLASS_TYPE(WindowResize)
private:
	int m_Width, m_Height;
};
