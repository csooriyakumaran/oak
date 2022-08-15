#include "oakpch.h"
#include "platform/OpenGL/OpenGLRendererAPI.h"

#include <glad/glad.h>

namespace Oak {
	
	void OpenGLMessageCallback(
		unsigned source,
		unsigned type,
		unsigned id,
		unsigned severity,
		int length,
		const char* message,
		const void* userParam)
	{
		switch (severity)
		{
			case GL_DEBUG_SEVERITY_HIGH:         OAK_CORE_CRITICAL(message); return;
			case GL_DEBUG_SEVERITY_MEDIUM:       OAK_CORE_ERROR(message); return;
			case GL_DEBUG_SEVERITY_LOW:          OAK_CORE_WARN(message); return;
			case GL_DEBUG_SEVERITY_NOTIFICATION: OAK_CORE_TRACE(message); return;
		}
		
		// HZ_CORE_CRITIAL(false, "Unknown severity leve!");
	}

	void OpenGLRendererAPI::Init()
	{

	#ifdef OAK_DEBUG
		glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		glDebugMessageCallback(OpenGLMessageCallback, nullptr);
		
		glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, NULL, GL_FALSE);
	#endif

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glEnable(GL_DEPTH_TEST);
		glEnable(GL_LINE_SMOOTH);
	}

	void OpenGLRendererAPI::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
	{
		glViewport(x, y, width, height);
	}

	void OpenGLRendererAPI::SetClearColor(const glm::vec4& color)
	{
		glClearColor(color.r, color.g, color.b, color.a);
	}

	void OpenGLRendererAPI::Clear()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void OpenGLRendererAPI::SetLineWidth(float width)
	{
		glLineWidth(width);
	}

}