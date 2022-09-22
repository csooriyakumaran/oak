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

		OAK_CORE_INFO("OpenGL Info:");
		OAK_CORE_INFO("  Vendor: {0}", glGetString(GL_VENDOR));
		OAK_CORE_INFO("  Renderer: {0}", glGetString(GL_RENDERER));
		OAK_CORE_INFO("  Version: {0}", glGetString(GL_VERSION));

	}

	void OpenGLContext::SwapBuffers()
	{

		glfwSwapBuffers(m_WindowHandle);
	}

}
