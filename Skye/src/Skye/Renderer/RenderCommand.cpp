#include "skpch.hpp"
#include "RenderCommand.hpp"

#include "Platform/OpenGL/OpenGLRendererAPI.hpp"

namespace Skye {

	RendererAPI* RenderCommand::s_RendererAPI = new OpenGLRendererAPI;
}