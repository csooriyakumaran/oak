#include "oak/renderer/GraphicsContext.h"

// #include "oak/renderer/Renderer.h"
#include "platform/OpenGL/OpenGLContext.h"

namespace Oak {

	std::unique_ptr<GraphicsContext> GraphicsContext::Create(void* window)
	{
		// switch (Renderer::GetAPI())
		// {
		// 	case RendererAPI::API::None:    HZ_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
		// 	case RendererAPI::API::OpenGL:  return CreateScope<OpenGLContext>(static_cast<GLFWwindow*>(window));
		// }

		return std::make_unique<OpenGLContext>(static_cast<GLFWwindow*>(window));

		// HZ_CORE_ASSERT(false, "Unknown RendererAPI!");
		// return nullptr;
	}

}