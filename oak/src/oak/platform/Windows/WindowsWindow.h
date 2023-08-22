#pragma once

#include "oak/Core/Window.h"
#include "oak/renderer/GraphicsContext.h"

#include <GLFW/glfw3.h>

namespace Oak {

	class WindowsWindow : public Window
	{
	public:
		WindowsWindow(const WindowSpecification& props);
		virtual ~WindowsWindow();

		virtual void Init() override;
		void OnUpdate() override;

		unsigned int GetWidth() const override { return m_Data.Width; }
		unsigned int GetHeight() const override { return m_Data.Height; }
		unsigned int GetPosX() const override { return m_Data.x; }
		unsigned int GetPosY() const override { return m_Data.y; }
		

		// Window attributes
		void SetEventCallback(const EventCallbackFn& callback) override { m_Data.EventCallback = callback; }
		void SetVSync(bool enabled) override;
		void SetResizable(bool resizable) const override;
		void CenterWindow() override;
		bool IsVSync() const override { return m_Data.VSync; }
		bool IsMaximized() const override { return (bool)glfwGetWindowAttrib(m_Window, GLFW_MAXIMIZED); }
		void Minimize() override;
		void Maximize() override;
		void Restore() override;
		void MoveDelta(int dx, int dy) override;
		void Move(int x, int y) override;


		virtual void* GetNativeWindow() const { return m_Window; }


	private:
		virtual void Shutdown();
	private:
		GLFWwindow* m_Window;
		GLFWcursor* m_ImGuiMouseCursors[9] = { 0 };
		WindowSpecification m_Specification;
		std::unique_ptr<GraphicsContext> m_Context;


		struct WindowData
		{
			std::string Title;
			unsigned int Width, Height;
			unsigned int x = 0, y = 0;
			bool VSync;
			bool Minimized;
			bool Maximized;
			bool Restored;

			EventCallbackFn EventCallback;
		};

		WindowData m_Data;
	};

}