#include "oakpch.h"
#include "oak/renderer/GraphicsContext.h"

#include "oak/renderer/Renderer.h"
#include "oak/platform/OpenGL/OpenGLContext.h"

namespace Oak {

	std::unique_ptr<GraphicsContext> GraphicsContext::Create(void* window)
	{
		 switch (Renderer::GetAPI())
		 {
		 	case RendererAPI::API::None:    CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
			case RendererAPI::API::OpenGL:  return wi::CreateScope<OpenGLContext>(static_cast<GLFWwindow*>(window));
		 }


		CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

}