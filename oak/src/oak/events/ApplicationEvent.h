#pragma once

#include "oak/events/Event.h"

namespace Oak {

	class WindowResizeEvent : public Event
	{
	public:
		WindowResizeEvent(unsigned int width, unsigned int height)
			: m_Width(width), m_Height(height) {}

		unsigned int GetWidth() const { return m_Width; }
		unsigned int GetHeight() const { return m_Height; }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "WindowResizeEvent: " << m_Width << ", " << m_Height;
			return ss.str();
		}

		EVENT_CLASS_TYPE(WindowResize)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
	private:
		unsigned int m_Width, m_Height;
	};

	class WindowCloseEvent : public Event
	{
	public:
		WindowCloseEvent() = default;
		
		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "WindowClose Event";
			return ss.str();
		}

		EVENT_CLASS_TYPE(WindowClose)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};


	class WindowMinimizeEvent : public Event
	{
	public:
		WindowMinimizeEvent() = default;

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "WindowMinimize Event";
			return ss.str();
		}

		EVENT_CLASS_TYPE(WindowMinimize)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};

	class WindowMaximizeEvent : public Event
	{
	public:
		WindowMaximizeEvent() = default;

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "WindowMaximize Event";
			return ss.str();
		}

		EVENT_CLASS_TYPE(WindowMaximize)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};

	class WindowRestoreEvent : public Event
	{
	public:
		WindowRestoreEvent() = default;
		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "WindowRestore Event";
			return ss.str();
		}

		EVENT_CLASS_TYPE(WindowRestore)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)

	};

	class WindowMovedEvent : public Event
	{
	public:
		WindowMovedEvent(int xpos, int ypos)
			: m_xpos(xpos), m_ypos(ypos) {}
		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "WindowMovedEvent: (" << m_xpos << ", " << m_xpos << ")";
			return ss.str();
		}

		EVENT_CLASS_TYPE(WindowRestore)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)

	private:
		unsigned int m_xpos, m_ypos;
	};

	class AppTickEvent : public Event
	{
	public:
		AppTickEvent() = default;

		EVENT_CLASS_TYPE(AppTick)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};

	class AppUpdateEvent : public Event
	{
	public:
		AppUpdateEvent() = default;

		EVENT_CLASS_TYPE(AppUpdate)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};

	class AppRenderEvent : public Event
	{
	public:
		AppRenderEvent() = default;

		EVENT_CLASS_TYPE(AppRender)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};
}