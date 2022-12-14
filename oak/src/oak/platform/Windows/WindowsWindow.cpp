#include "oakpch.h"
#include "oak/core/Base.h"

#include "oak/platform/Windows/WindowsWindow.h"

#include "oak/core/Input.h"

#include "oak/events/ApplicationEvent.h"
#include "oak/events/MouseEvent.h"
#include "oak/events/KeyEvent.h"

#include "oak/renderer/Renderer.h"

#include "oak/platform/OpenGL/OpenGLContext.h"
#include "stb_image.h"

namespace Oak {

    static uint8_t s_GLFWWindowCount = 0;

    static void GLFWErrorCallback(int error, const char* description)
	{
	}

    WindowsWindow::WindowsWindow(const WindowProps& props)
    {
        Init(props);
    }
    
    WindowsWindow::~WindowsWindow()
    {
        Shutdown();
    }

	void WindowsWindow::Init(const WindowProps& props)
	{
		m_Data.Title = props.Title;
		m_Data.Width = props.Width;
		m_Data.Height = props.Height;
		OAK_CORE_INFO("Creating window {0} ({1}, {2})", props.Title, props.Width, props.Height);

		if (s_GLFWWindowCount == 0)
		{
			int success = glfwInit();
			OAK_CORE_ASSERT(success, "GLFW failed to initialize");
			glfwSetErrorCallback(GLFWErrorCallback);
		}

		{
#if defined(OAK_DEBUG)
			if (Renderer::GetAPI() == RendererAPI::API::OpenGL)
				glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
#endif
			glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);
			glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
			m_Window = glfwCreateWindow((int)props.Width, (int)props.Height, m_Data.Title.c_str(), nullptr, nullptr);
			++s_GLFWWindowCount;

			GLFWimage images[1];
			//images[0].pixels = stbi_load("assets/textures/icons8-opium-poppy-24.png", &images[0].width, &images[0].height, 0, 4); //rgba channels 
			images[0].pixels = stbi_load("assets/textures/wi-logo-gradient-2-24.png", &images[0].width, &images[0].height, 0, 4); //rgba channels 
			glfwSetWindowIcon(m_Window, 1, images);
			stbi_image_free(images[0].pixels);

		
		
		}



		m_Context = GraphicsContext::Create(m_Window);
		m_Context->Init();

		glfwSetWindowUserPointer(m_Window, &m_Data);
		SetVSync(true);

		// Set GLFW callbacks
		glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			data.Width = width;
			data.Height = height;

			WindowResizeEvent event(width, height);
			data.EventCallback(event);
		});

		glfwSetWindowPosCallback(m_Window, [](GLFWwindow* window, int xpos, int ypos)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			data.x = xpos;
			data.y = ypos;
			WindowMovedEvent event(xpos, ypos);
			data.EventCallback(event);

		});

		glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			WindowCloseEvent event;
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
				data.Restored  = false;
				data.EventCallback(event);

			} else {
				data.Minimized = false;
				if (glfwGetWindowAttrib(window, GLFW_MAXIMIZED))
				{
					data.Maximized = true;
					data.Restored = false;
					WindowMaximizeEvent event;
					data.EventCallback(event);

				} else {
					data.Maximized = false;
					data.Restored  = true;
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
				data.Restored  = false;
				data.EventCallback(event);
			} else {
				WindowRestoreEvent event;
				data.Minimized = false;
				data.Maximized = false;
				data.Restored = true;
				data.EventCallback(event);
			}

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
	void WindowsWindow::Restore()
	{
		glfwRestoreWindow(m_Window);
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
