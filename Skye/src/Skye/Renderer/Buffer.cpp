#include "skpch.hpp"
#include "Buffer.hpp"

#include "Renderer.hpp"

// TODO: Include based on Rendering API (i.e. DirectX, OpenGL, Vulkan, etc.)
#include "Platform/OpenGL/OpenGLBuffer.hpp"

namespace Skye {

	Ref<VertexBuffer> VertexBuffer::Create(uint32_t size)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:		SK_CORE_ASSERT(false, "RendererAPI::NONE is not currently supported!");  return nullptr;
		case RendererAPI::API::OpenGL:		return CreateRef<OpenGLVertexBuffer>(size);
		}

		SK_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

	Ref<VertexBuffer> VertexBuffer::Create(float* vertices, uint32_t size)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:		SK_CORE_ASSERT(false, "RendererAPI::NONE is not currently supported!");  return nullptr;
			case RendererAPI::API::OpenGL:	return CreateRef<OpenGLVertexBuffer>(vertices, size);
		}

		SK_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

	Ref<IndexBuffer> IndexBuffer::Create(uint32_t* indices, uint32_t count)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None:		SK_CORE_ASSERT(false, "RendererAPI::NONE is not currently supported!");  return nullptr;
			case RendererAPI::API::OpenGL:		return CreateRef<OpenGLIndexBuffer>(indices, count);
		}

		SK_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
	
}