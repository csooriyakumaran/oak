
#pragma once


#include <sstream>
#include <functional>

#include "oak/events/event.h"

namespace Oak {

	struct WindowSpecification
	{
		std::string Title = "Wallace Instruments";
		uint32_t Width = 1600;
		uint32_t Height = 900;
		bool Decorated = true;
		bool Fullscreen = false;
		bool VSync = true;
	};

	// Interface representing a desktop system based Window
	class Window
	{
	public:
		using EventCallbackFn = std::function<void(Event&)>;

		virtual ~Window() = default;

		virtual void Init() = 0;
		virtual void OnUpdate() = 0;

		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;
		virtual uint32_t GetPosX() const = 0 ;
		virtual uint32_t GetPosY() const = 0 ;

		// Window attributes
		virtual void SetEventCallback(const EventCallbackFn& callback) = 0;
		virtual void SetVSync(bool enabled) = 0;
		virtual void SetResizable(bool resizable) const = 0;
		virtual bool IsVSync() const = 0;
		virtual bool IsMaximized() const = 0;
		virtual void Minimize() = 0;
		virtual void Maximize() = 0;
		virtual void CenterWindow() = 0;
		virtual void Restore() = 0;
		virtual void Move(int x, int y) = 0;
		virtual void MoveDelta(int dx, int dy) = 0;

		virtual void* GetNativeWindow() const = 0;

		static Window* Create(const WindowSpecification& spec = WindowSpecification());
	};

}