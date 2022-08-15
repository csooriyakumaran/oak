#pragma once
#include "oakpch.h"
#include "oak/renderer/GraphicsContext.h"


#include <Glad/glad.h>
#include <GLFW/glfw3.h>

struct GLFWwindow;

namespace Oak {

	class OpenGLContext : public GraphicsContext
	{
	public:
		OpenGLContext(GLFWwindow* windowHandle);

		virtual void Init() override;
		virtual void SwapBuffers() override;
	private:
		GLFWwindow* m_WindowHandle;
	};

}