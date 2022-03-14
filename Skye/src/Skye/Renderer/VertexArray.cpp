#include "skpch.hpp"
#include "VertexArray.hpp"

#include "Renderer.hpp"
#include "Platform/OpenGL/OpenGLVertexArray.hpp"

namespace Skye {

	VertexArray* VertexArray::Create()
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None:		SK_CORE_ASSERT(false, "RendererAPI::NONE is not currently supported!");  return nullptr;
			case RendererAPI::API::OpenGL:	return new OpenGLVertexArray();
		}

		SK_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
}