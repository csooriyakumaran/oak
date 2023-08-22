#include "oakpch.h"
#include "oak/core/Base.h"
#include "oak/platform/OpenGL/OpenGLContext.h"

#include <Glad/glad.h>
#include <GLFW/glfw3.h>

namespace Oak {

	OpenGLContext::OpenGLContext(GLFWwindow* windowHandle)
		: m_WindowHandle(windowHandle)
	{
        
	}

	void OpenGLContext::Init()
	{

		glfwMakeContextCurrent(m_WindowHandle);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

		LOG_CORE_INFO("OpenGL Info:");
		LOG_CORE_INFO("  Vendor: {0}",   reinterpret_cast<const char*>(glGetString(GL_VENDOR)));
		LOG_CORE_INFO("  Renderer: {0}", reinterpret_cast<const char*>(glGetString(GL_RENDERER)));
		LOG_CORE_INFO("  Version: {0}",  reinterpret_cast<const char*>(glGetString(GL_VERSION)));

	}

	void OpenGLContext::SwapBuffers()
	{

		glfwSwapBuffers(m_WindowHandle);
	}

}
