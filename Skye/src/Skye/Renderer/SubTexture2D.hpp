#pragma once

#include <glm/glm.hpp>
#include "Texture.hpp"

namespace Skye {

	class SubTexture2D
	{
	public:
		SubTexture2D(const Ref<Texture2D>& texture, const glm::vec2& min, const glm::vec2& max);

		static Ref<SubTexture2D> CreateFromCoords(const Ref<Texture2D>& texture, const glm::vec2& coords, const glm::vec2& spriteSize = { 128.0f, 128.0f });

	private:
		Ref<Texture2D> m_Texture;

		glm::vec2 m_TexCoords[4];

	public:
		FORCEINLINE const glm::vec2* GetTexCoords() const { return m_TexCoords; }
		FORCEINLINE const Ref<Texture2D> GetTexture() const { return m_Texture; }
	};
}