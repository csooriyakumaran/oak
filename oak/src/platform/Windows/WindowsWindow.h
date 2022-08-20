#pragma once

#include "oak/Core/Window.h"
#include "oak/renderer/GraphicsContext.h"

#include <GLFW/glfw3.h>

namespace Oak {

	class WindowsWindow : public Window
	{
	public:
		WindowsWindow(const WindowProps& props);
		virtual ~WindowsWindow();

		void OnUpdate() override;

		unsigned int GetWidth() const override { return m_Data.Width; }
		unsigned int GetHeight() const override { return m_Data.Height; }
		unsigned int GetPosX() const override { return m_Data.x; }
		unsigned int GetPosY() const override { return m_Data.y; }
		

		// Window attributes
		void SetEventCallback(const EventCallbackFn& callback) override { m_Data.EventCallback = callback; }
		void SetVSync(bool enabled) override;
		bool IsVSync() const override { return m_Data.VSync; }
		void Minimize() override;
		void Maximize() override;
		void Restore() override;
		void Move(int dx, int dy) override;


		virtual void* GetNativeWindow() const { return m_Window; }


	private:
		virtual void Init(const WindowProps& props);
		virtual void Shutdown();
	private:
		GLFWwindow* m_Window;
		std::unique_ptr<GraphicsContext> m_Context;

		struct WindowData
		{
			std::string Title;
			unsigned int Width, Height, x, y;
			bool VSync;
			bool Minimized;
			bool Maximized;
			bool Restored;

			EventCallbackFn EventCallback;
		};

		WindowData m_Data;
	};

}