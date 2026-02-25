#include "ldpch.h"
#include "RenderCommand.h"

#include "Platform/OpenGL/OpenGLRendererAPI.h"

namespace LazyDog
{
	RendererAPI* RenderCommand::s_RendererAPI = new OpenGLRendererAPI();
}