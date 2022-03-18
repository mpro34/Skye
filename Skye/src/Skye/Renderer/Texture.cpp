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
		case RendererAPI::API::OpenGL:		return std::make_shared<OpenGLTexture2D>(path);
		}

		SK_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
}