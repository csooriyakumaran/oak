#include "oakpch.h"
#include "oak/renderer/RenderCommand.h"

namespace Oak {

	wi::Scope<RendererAPI> RenderCommand::s_RendererAPI = RendererAPI::Create();

}