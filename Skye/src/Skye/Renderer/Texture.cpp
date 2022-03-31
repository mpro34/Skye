#include "skpch.hpp"
#include "Texture.hpp"

#include "Renderer.hpp"
#include "Platform/OpenGL/OpenGLTexture.hpp"

namespace Skye {
	
	Ref<Texture2D> Texture2D::Create(const std::string& path)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None:		SK_CORE_ASSERT(false, "RendererAPI::NONE is not currently supported!");  return nullptr;
			case RendererAPI::API::OpenGL:		return CreateRef<OpenGLTexture2D>(path);
		}

		SK_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

	Ref<Texture2D> Texture2D::Create(uint32_t width, uint32_t height)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None:		SK_CORE_ASSERT(false, "RendererAPI::NONE is not currently supported!");  return nullptr;
			case RendererAPI::API::OpenGL:		return CreateRef<OpenGLTexture2D>(width, height);
		}

		SK_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
}