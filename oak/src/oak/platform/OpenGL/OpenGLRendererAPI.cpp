#include "oakpch.h"
#include "oak/platform/OpenGL/OpenGLRendererAPI.h"

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
			case GL_DEBUG_SEVERITY_HIGH:         LOG_CORE_FATAL_TAG("OpenGLRendererAPI", message); return;
			case GL_DEBUG_SEVERITY_MEDIUM:       LOG_CORE_ERROR_TAG("OpenGLRendererAPI", message); return;
			case GL_DEBUG_SEVERITY_LOW:          LOG_CORE_WARN_TAG("OpenGLRendererAPI", message); return;
			case GL_DEBUG_SEVERITY_NOTIFICATION: LOG_CORE_TRACE_TAG("OpenGLRendererAPI", message); return;
		}
		
		// HZ_CORE_CRITIAL(false, "Unknown severity leve!");
	}

	void OpenGLRendererAPI::Init()
	{

	#ifdef WI_DEBUG
		glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		glDebugMessageCallback(OpenGLMessageCallback, nullptr);
		
		glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, NULL, GL_FALSE);
	#endif // WI_DEBUG

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