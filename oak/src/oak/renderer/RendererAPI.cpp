#include "oakpch.h"
#include "oak/renderer/RendererAPI.h"

#include "oak/platform/OpenGL/OpenGLRendererAPI.h"

namespace Oak {

	RendererAPI::API RendererAPI::s_API = RendererAPI::API::OpenGL;

	Scope<RendererAPI> RendererAPI::Create()
	{
		switch (s_API)
		{
			case RendererAPI::API::None:    OAK_CORE_FATAL_TAG("RendererAPI", "RendererAPI::None is currently not supported!"); return nullptr;
			case RendererAPI::API::OpenGL:  return CreateScope<OpenGLRendererAPI>();
		}

		OAK_CORE_FATAL_TAG("RendererAPI", "Unknown RendererAPI!");
		return nullptr;
	}

}