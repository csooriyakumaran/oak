#include "oakpch.h"
#include "oak/core/Base.h"

#include "oak/platform/Windows/WindowsWindow.h"

#include "oak/core/Input.h"

#include "oak/events/ApplicationEvent.h"
#include "oak/events/MouseEvent.h"
#include "oak/events/KeyEvent.h"

#include "oak/renderer/Renderer.h"

#include "oak/platform/OpenGL/OpenGLContext.h"
#include "imgui.h"
#include "stb_image.h"

namespace Oak {

    static void GLFWErrorCallback(int error, const char* description)
	{
		LOG_CORE_ERROR_TAG("GLFW", "GLFW Error ({0}): {1}", error, description);
	}

    static uint8_t s_GLFWWindowCount = 0;

	Window* Window::Create(const WindowSpecification& specification)
	{
		return new WindowsWindow(specification);
	}

	WindowsWindow::WindowsWindow(const WindowSpecification& specification)
		: m_Specification(specification)
	{
	}

	WindowsWindow::~WindowsWindow()
	{
		Shutdown();
	}

	void WindowsWindow::Init()
	{
		m_Data.Title = m_Specification.Title;
		m_Data.Width = m_Specification.Width;
		m_Data.Height = m_Specification.Height;

		LOG_CORE_INFO("Creating window {0} ({1}, {2})", m_Specification.Title, m_Specification.Width, m_Specification.Height);

		if (s_GLFWWindowCount == 0)
		{
			// TODO: glfwTerminate on system shutdown
			int success = glfwInit();
			CORE_ASSERT(success, "GLFW failed to initialize");
			glfwSetErrorCallback(GLFWErrorCallback);
		}

		if (Renderer::GetAPI() == RendererAPI::API::OpenGL)
		{
			glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
#ifdef WI_DEBUG
			glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
#endif
		}

		if (!m_Specification.Decorated)
		{
			// removes the title bar on all platforms and all of the native window effects on non-Windows platforms. 
#ifdef WI_PLATFORM_WINDOWS
			glfwWindowHint(GLFW_TITLEBAR, false);
#else
			glfwWindowHint(GLFW_DECORATED, false);
#endif
		}

		if (m_Specification.Fullscreen)
		{
			GLFWmonitor* primaryMonitor = glfwGetPrimaryMonitor();
			const GLFWvidmode* mode = glfwGetVideoMode(primaryMonitor);
			
			glfwWindowHint(GLFW_DECORATED, false);
			glfwWindowHint(GLFW_RED_BITS, mode->redBits);
			glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
			glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
			glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);

			m_Window = glfwCreateWindow(mode->width, mode->height, m_Data.Title.c_str(), primaryMonitor, nullptr);
			++s_GLFWWindowCount;
		}
		else
		{
			m_Window = glfwCreateWindow((int)m_Specification.Width, (int)m_Specification.Height, m_Data.Title.c_str(), nullptr, nullptr);
			++s_GLFWWindowCount;
		}

		m_Context = GraphicsContext::Create(m_Window);
		m_Context->Init();

		glfwSetWindowUserPointer(m_Window, &m_Data);
		SetVSync(m_Specification.VSync);

		// Set GLFW callbacks
		glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			WindowResizeEvent event(width, height);
			data.EventCallback(event);
			data.Width = width;
			data.Height = height;
		});

		glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			
			WindowCloseEvent event;
			data.EventCallback(event);
		});

		glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

				switch (action)
				{
				case GLFW_PRESS:
				{
					KeyPressedEvent event(key, 0);
					data.EventCallback(event);
					break;
				}
				case GLFW_RELEASE:
				{
					KeyReleasedEvent event(key);
					data.EventCallback(event);
					break;
				}
				case GLFW_REPEAT:
				{
					KeyPressedEvent event(key, true);
					data.EventCallback(event);
					break;
				}
				}
			});

		glfwSetCharCallback(m_Window, [](GLFWwindow* window, unsigned int keycode)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			KeyTypedEvent event(keycode);
			data.EventCallback(event);
		});

		glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int button, int action, int mods)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			switch (action)
			{
				case GLFW_PRESS:
				{
					MouseButtonPressedEvent event(button);
					data.EventCallback(event);
					break;
				}
				case GLFW_RELEASE:
				{
					MouseButtonReleasedEvent event(button);
					data.EventCallback(event);
					break;
				}
			}
		});

		glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double xOffset, double yOffset)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			MouseScrolledEvent event((float)xOffset, (float)yOffset);
			data.EventCallback(event);
		});

		glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double xPos, double yPos)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			MouseMovedEvent event((float)xPos, (float)yPos);
			data.EventCallback(event);
		});

		glfwSetTitlebarHitTestCallback(m_Window, [](GLFWwindow* window, int x, int y, int* hit)
		{
			auto& data = *((WindowData*)glfwGetWindowUserPointer(window));
			WindowTitleBarHitTestEvent event(x, y, *hit);
			data.EventCallback(event);
		});


    	m_ImGuiMouseCursors[ImGuiMouseCursor_Arrow] = glfwCreateStandardCursor(GLFW_ARROW_CURSOR);
		m_ImGuiMouseCursors[ImGuiMouseCursor_TextInput] = glfwCreateStandardCursor(GLFW_IBEAM_CURSOR);
		m_ImGuiMouseCursors[ImGuiMouseCursor_ResizeAll] = glfwCreateStandardCursor(GLFW_ARROW_CURSOR);   // FIXME: GLFW doesn't have this.
		m_ImGuiMouseCursors[ImGuiMouseCursor_ResizeNS] = glfwCreateStandardCursor(GLFW_VRESIZE_CURSOR);
		m_ImGuiMouseCursors[ImGuiMouseCursor_ResizeEW] = glfwCreateStandardCursor(GLFW_HRESIZE_CURSOR);
		m_ImGuiMouseCursors[ImGuiMouseCursor_ResizeNESW] = glfwCreateStandardCursor(GLFW_ARROW_CURSOR);  // FIXME: GLFW doesn't have this.
		m_ImGuiMouseCursors[ImGuiMouseCursor_ResizeNWSE] = glfwCreateStandardCursor(GLFW_ARROW_CURSOR);  // FIXME: GLFW doesn't have this.
		m_ImGuiMouseCursors[ImGuiMouseCursor_Hand] = glfwCreateStandardCursor(GLFW_HAND_CURSOR);


		glfwSetWindowPosCallback(m_Window, [](GLFWwindow* window, int xpos, int ypos)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
				data.x = xpos;
				data.y = ypos;
				WindowMovedEvent event(xpos, ypos);
				data.EventCallback(event);

			});

		glfwSetWindowIconifyCallback(m_Window, [](GLFWwindow* window, int iconified)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
				if (iconified)
				{
					WindowMinimizeEvent event;
					data.Minimized = true;
					data.Maximized = false;
					data.Restored = false;
					data.EventCallback(event);

				}
				else {
					data.Minimized = false;
					if (glfwGetWindowAttrib(window, GLFW_MAXIMIZED))
					{
						data.Maximized = true;
						data.Restored = false;
						WindowMaximizeEvent event;
						data.EventCallback(event);

					}
					else {
						data.Maximized = false;
						data.Restored = true;
						WindowRestoreEvent event;
						data.EventCallback(event);
					}
				}
			});

		glfwSetWindowMaximizeCallback(m_Window, [](GLFWwindow* window, int maximized)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
				if (maximized)
				{
					WindowMaximizeEvent event;
					data.Minimized = false;
					data.Maximized = true;
					data.Restored = false;
					data.EventCallback(event);
				}
				else {
					WindowRestoreEvent event;
					data.Minimized = false;
					data.Maximized = false;
					data.Restored = true;
					data.EventCallback(event);
				}

			});




		// update window size to actual size
		{
			int width, height;
			glfwGetWindowSize(m_Window, &width, &height);
			m_Data.Width = width;
			m_Data.Height = height;
		}

		// set icon
		{
			GLFWimage icon;
			int channels;
			icon.pixels = stbi_load("assets/textures/wi-logo-gradient-2-24.png", &icon.width, &icon.height, &channels, 4);
			glfwSetWindowIcon(m_Window, 1, &icon);
			stbi_image_free(icon.pixels);
		}




    }
   
	void WindowsWindow::Shutdown()
    {

		glfwDestroyWindow(m_Window);
		--s_GLFWWindowCount;

		if (s_GLFWWindowCount == 0)
		{
			glfwTerminate();
		}
	}

	void WindowsWindow::OnUpdate()
	{
		glfwPollEvents();
		m_Context->SwapBuffers();
	}

	void WindowsWindow::SetVSync(bool enabled)
	{
		m_Specification.VSync = enabled;

		if (enabled)
			glfwSwapInterval(1);
		else
			glfwSwapInterval(0);

		m_Data.VSync = enabled;
	}

	void WindowsWindow::Minimize()
	{
		glfwIconifyWindow(m_Window);
	}
	
	void WindowsWindow::Maximize()
	{
		glfwMaximizeWindow(m_Window);
	}

	void WindowsWindow::CenterWindow()
	{
		const GLFWvidmode* videmode = glfwGetVideoMode(glfwGetPrimaryMonitor());
		int x = (videmode->width / 2) - (m_Data.Width / 2);
		int y = (videmode->height / 2) - (m_Data.Height / 2);
		glfwSetWindowPos(m_Window, x, y);
	}

	void WindowsWindow::Restore()
	{
		glfwRestoreWindow(m_Window);
	}

	void WindowsWindow::SetResizable(bool resizable) const
	{
		glfwSetWindowAttrib(m_Window, GLFW_RESIZABLE, resizable ? GLFW_TRUE : GLFW_FALSE);
	}


	void WindowsWindow::MoveDelta(int dx, int dy)
	{
		int x_current, y_current;
		glfwGetWindowPos(m_Window, &x_current, &y_current);
		glfwSetWindowPos(m_Window, x_current + dx, y_current + dy);
	}

	void WindowsWindow::Move(int x, int y)
	{
		glfwSetWindowPos(m_Window,x, y);
	}

	
}
