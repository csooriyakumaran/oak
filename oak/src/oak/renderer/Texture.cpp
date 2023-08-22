#include "oakpch.h"
#include "oak/renderer/Texture.h"

#include "oak/renderer/Renderer.h"
#include "oak/platform/OpenGL/OpenGLTexture.h"

namespace Oak {
	using namespace wi;
	Ref<Texture2D> Texture2D::Create(uint32_t width, uint32_t height)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None:    CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
			case RendererAPI::API::OpenGL:  return Ref<OpenGLTexture2D>::Create(width, height);
		}

		CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

	Ref<Texture2D> Texture2D::Create(const std::string& path)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None:    CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
			case RendererAPI::API::OpenGL:  return Ref<OpenGLTexture2D>::Create(path);
		}

		CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

}