#include "skpch.hpp"
#include "OpenGLTexture.hpp"

#include <glad/glad.h>
#include "stb_image.h"

namespace Skye {

	OpenGLTexture2D::OpenGLTexture2D(const std::string& path)
		:m_Path(path)
	{
		SK_PROFILE_FUNCTION();

		int width, height, channels;
		stbi_set_flip_vertically_on_load(1);
		stbi_uc* data = nullptr;
		{
			SK_PROFILE_SCOPE("stbi_load - OpenGLTexture2D");
			data = stbi_load(path.c_str(), &width, &height, &channels, 0);
		}
		SK_CORE_ASSERT(data, "Failed to load image with stb_image!");
		m_Width = width;
		m_Height = height;

		// Based on the number of channels in the image, determine how to store the pixels and how to render them.
		GLenum InternalFormat = 0, DataFormat = 0;
		if (channels == 4)
		{
			InternalFormat = GL_RGBA8;
			DataFormat = GL_RGBA;
		}
		else if (channels == 3)
		{
			InternalFormat = GL_RGBA8;
			DataFormat = GL_RGB;
		}

		SK_CORE_ASSERT(InternalFormat & DataFormat, "Format not supported for Texture!");
		m_InternalFormat = InternalFormat;
		m_DataFormat = DataFormat;

		glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
		glTextureStorage2D(m_RendererID, 1, m_InternalFormat, m_Width, m_Height);

		glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);

		// Upload texture data to GPU
		glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, m_DataFormat, GL_UNSIGNED_BYTE, data);

		stbi_image_free(data); // Free memory from CPU after storing on GPU
	}

	OpenGLTexture2D::OpenGLTexture2D(uint32_t width, uint32_t height)
		: m_Width(width), m_Height(height)
	{
		SK_PROFILE_FUNCTION();

		// Based on the number of channels in the image, determine how to store the pixels and how to render them.
		m_InternalFormat = GL_RGBA8;
		m_DataFormat = GL_RGBA;

		glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
		glTextureStorage2D(m_RendererID, 1, m_InternalFormat, m_Width, m_Height);

		glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);
	}

	OpenGLTexture2D::~OpenGLTexture2D()
	{
		SK_PROFILE_FUNCTION();

		glDeleteTextures(1, &m_RendererID);
	}

	void OpenGLTexture2D::SetData(void* data, uint32_t size)
	{
		SK_PROFILE_FUNCTION();

		glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, m_DataFormat, GL_UNSIGNED_BYTE, data);
	}

	void OpenGLTexture2D::Bind(uint32_t slot) const
	{
		SK_PROFILE_FUNCTION();

		glBindTextureUnit(slot, m_RendererID);
	}
}