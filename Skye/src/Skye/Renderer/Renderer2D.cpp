#include "skpch.hpp"
#include "Renderer2D.hpp"

#include "VertexArray.hpp"
#include "Shader.hpp"
#include "RenderCommand.hpp"

#include <glm/gtc/matrix_transform.hpp>

namespace Skye {

	struct QuadVertex
	{
		glm::vec3 Position;
		glm::vec4 Color;
		glm::vec2 TexCoord;
		float TexIndex;
		float TilingFactor;
	};

	struct Renderer2DData
	{
		// Max quads to draw in a single draw call
		static const uint32_t MAX_QUADS = 20000;
		static const uint32_t MAX_VERTICES = MAX_QUADS * 4;
		static const uint32_t MAX_INDICES = MAX_QUADS * 6;
		static const uint32_t MAX_TEXTURE_SLOTS = 32; // RenderCapabilities

		Ref<VertexArray> QuadVertexArray;
		Ref<VertexBuffer> QuadVertexBuffer;
		Ref<Shader> TextureShader;
		Ref<Texture2D> WhiteTexture;

		uint32_t QuadIndexCount = 0;
		QuadVertex* QuadVertexBufferBase = nullptr;
		QuadVertex* QuadVertexBufferPtr = nullptr;

		// Store unique ids of each of the 32 texture slots that are bound
		std::array<Ref<Texture2D>, MAX_TEXTURE_SLOTS> TextureSlots;
		uint32_t TexutreSlotIndex = 1;

		glm::vec4 QuadVertexPositions[4];

		Renderer2D::Statistics Stats;
	};

	static Renderer2DData s_2DData;

	void Renderer2D::Init()
	{
		SK_PROFILE_FUNCTION();

		// Make pointer to static struct to ensure vertex array and shader data destructors are called.
		s_2DData.QuadVertexArray = VertexArray::Create();

		s_2DData.QuadVertexBuffer = VertexBuffer::Create(s_2DData.MAX_VERTICES * sizeof(QuadVertex));
		s_2DData.QuadVertexBuffer->SetLayout({
			{ShaderDataType::Float3, "a_Position"},
			{ShaderDataType::Float4, "a_Color"},
			{ShaderDataType::Float2, "a_TexCoord"},
			{ShaderDataType::Float,  "a_TexIndex"},
			{ShaderDataType::Float,  "a_TilingFactor"}
		});
		s_2DData.QuadVertexArray->AddVertexBuffer(s_2DData.QuadVertexBuffer);

		s_2DData.QuadVertexBufferBase = new QuadVertex[s_2DData.MAX_VERTICES];

		uint32_t* quadIndicies = new uint32_t[s_2DData.MAX_INDICES];

		uint32_t offset = 0;
		for (uint32_t i = 0; i < s_2DData.MAX_INDICES; i += 6)
		{
			quadIndicies[i + 0] = offset + 0;
			quadIndicies[i + 1] = offset + 1;
			quadIndicies[i + 2] = offset + 2;

			quadIndicies[i + 3] = offset + 2;
			quadIndicies[i + 4] = offset + 3;
			quadIndicies[i + 5] = offset + 0;

			offset += 4;
		}

		Ref<IndexBuffer> quad_ib = IndexBuffer::Create(quadIndicies, s_2DData.MAX_INDICES);
		s_2DData.QuadVertexArray->SetIndexBuffer(quad_ib);
		delete[] quadIndicies;

		s_2DData.WhiteTexture = Texture2D::Create(1, 1);
		uint32_t whiteTextureData = 0xffffffff; // Full white texture
		s_2DData.WhiteTexture->SetData(&whiteTextureData, sizeof(whiteTextureData));

		int32_t samplers[s_2DData.MAX_TEXTURE_SLOTS];
		for (int32_t i = 0; i < s_2DData.MAX_TEXTURE_SLOTS; i++)
		{
			samplers[i] = i;
		}

		// Load shader from file
		s_2DData.TextureShader = Shader::Create("assets/shaders/Texture.glsl");
		s_2DData.TextureShader->Bind();
		s_2DData.TextureShader->SetIntArray("u_Textures", samplers, s_2DData.MAX_TEXTURE_SLOTS);

		// Set default white texture
		s_2DData.TextureSlots[0] = s_2DData.WhiteTexture;

		s_2DData.QuadVertexPositions[0] = { -0.5f, -0.5f, 0.0f, 1.0f };
		s_2DData.QuadVertexPositions[1] = { 0.5f, -0.5f, 0.0f, 1.0f };
		s_2DData.QuadVertexPositions[2] = { 0.5f, 0.5f, 0.0f, 1.0f };
		s_2DData.QuadVertexPositions[3] = { -0.5f, 0.5f, 0.0f, 1.0f };
	}
	
	void Renderer2D::Shutdown()
	{
		SK_PROFILE_FUNCTION();
	}

	void Renderer2D::BeginScene(const OrthographicCamera& camera)
	{
		SK_PROFILE_FUNCTION();

		s_2DData.TextureShader->Bind();
		s_2DData.TextureShader->SetMat4("u_ViewProjection", camera.GetViewProjectionMatrix());

		s_2DData.QuadIndexCount = 0;
		s_2DData.QuadVertexBufferPtr = s_2DData.QuadVertexBufferBase;

		s_2DData.TexutreSlotIndex = 1;
	}

	void Renderer2D::EndScene()
	{
		SK_PROFILE_FUNCTION();

		uint32_t dataSize = (uint8_t*)s_2DData.QuadVertexBufferPtr - (uint8_t*)s_2DData.QuadVertexBufferBase;
		s_2DData.QuadVertexBuffer->SetData(s_2DData.QuadVertexBufferBase, dataSize);

		Flush();
	}

	void Renderer2D::Flush()
	{
		// Bind textures
		for (uint32_t i = 0; i < s_2DData.TexutreSlotIndex; i++)
		{
			s_2DData.TextureSlots[i]->Bind(i);
		}
		RenderCommand::DrawIndexed(s_2DData.QuadVertexArray, s_2DData.QuadIndexCount);
		// Increment number of draw calls in statistics
		s_2DData.Stats.DrawCalls++;
	}

	/*
	* @brief This function is used to reset the index and texture slots if drawing more than 32 quads.
	* It differs from BeginScene(), because we don't need to rebind the texture shader or use the camera!
	*/
	void Renderer2D::FlushAndReset()
	{
		EndScene();

		s_2DData.QuadIndexCount = 0;
		s_2DData.QuadVertexBufferPtr = s_2DData.QuadVertexBufferBase;

		s_2DData.TexutreSlotIndex = 1;
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const float rotation_angle, const glm::vec2& size, const glm::vec4& color)
	{
		DrawQuad({ position.x, position.y, 0.0f }, rotation_angle, size, color);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const float rotation_angle, const glm::vec2& size, const glm::vec4& color)
	{
		SK_PROFILE_FUNCTION();

		if (s_2DData.QuadIndexCount >= Renderer2DData::MAX_INDICES)
		{
			FlushAndReset();
		}

		const float textureIndex = 0.0f; // default white texture
		const float tilingMultiplier = 1.0f;

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::rotate(glm::mat4(1.0f), rotation_angle, { 0.0f, 0.0f, 1.0f })
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		s_2DData.QuadVertexBufferPtr->Position = transform * s_2DData.QuadVertexPositions[0];
		s_2DData.QuadVertexBufferPtr->Color = color;
		s_2DData.QuadVertexBufferPtr->TexCoord = {0.0f, 0.0f};
		s_2DData.QuadVertexBufferPtr->TexIndex = textureIndex;
		s_2DData.QuadVertexBufferPtr->TilingFactor = tilingMultiplier;
		s_2DData.QuadVertexBufferPtr++;

		s_2DData.QuadVertexBufferPtr->Position = transform * s_2DData.QuadVertexPositions[1];
		s_2DData.QuadVertexBufferPtr->Color = color;
		s_2DData.QuadVertexBufferPtr->TexCoord = { 1.0f, 0.0f };
		s_2DData.QuadVertexBufferPtr->TexIndex = textureIndex;
		s_2DData.QuadVertexBufferPtr->TilingFactor = tilingMultiplier;
		s_2DData.QuadVertexBufferPtr++;

		s_2DData.QuadVertexBufferPtr->Position = transform * s_2DData.QuadVertexPositions[2];
		s_2DData.QuadVertexBufferPtr->Color = color;
		s_2DData.QuadVertexBufferPtr->TexCoord = { 0.0f, 1.0f };
		s_2DData.QuadVertexBufferPtr->TexIndex = textureIndex;
		s_2DData.QuadVertexBufferPtr->TilingFactor = tilingMultiplier;
		s_2DData.QuadVertexBufferPtr++;

		s_2DData.QuadVertexBufferPtr->Position = transform * s_2DData.QuadVertexPositions[3];
		s_2DData.QuadVertexBufferPtr->Color = color;
		s_2DData.QuadVertexBufferPtr->TexCoord = { 1.0f, 1.0f };
		s_2DData.QuadVertexBufferPtr->TexIndex = textureIndex;
		s_2DData.QuadVertexBufferPtr->TilingFactor = tilingMultiplier;
		s_2DData.QuadVertexBufferPtr++;

		s_2DData.QuadIndexCount += 6; // Add 6 more indices	

		// Increment stats
		s_2DData.Stats.QuadCount++;
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const float rotation_angle, const Ref<Texture2D>& texture, float tileMultiplier, const glm::vec4& tintColor)
	{
		DrawQuad({ position.x, position.y, 0.0f }, size, rotation_angle, texture, tileMultiplier, tintColor);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const float rotation_angle, const Ref<Texture2D>& texture, float tileMultiplier, const glm::vec4& tintColor)
	{
		SK_PROFILE_FUNCTION();

		// Texture location in spritesheet
		constexpr float x = 18, y = 1;
		constexpr float sheetWidth = 2944.0f, sheetHeight = 1664.0f;
		constexpr float spriteWidth = 128.0f, spriteHeight = 128.0f;

		if (s_2DData.QuadIndexCount >= Renderer2DData::MAX_INDICES)
		{
			FlushAndReset();
		}

		constexpr glm::vec4 color = { 1.0f, 1.0f, 1.0f, 1.0f };

		float textureIndex = 0.0f;

		for (uint32_t i = 1; i < s_2DData.TexutreSlotIndex; i++)
		{
			if (*s_2DData.TextureSlots[i].get() == *texture.get())
			{
				textureIndex = (float)i;
				break;
			}
		}

		if (textureIndex == 0.0f)
		{
			textureIndex = (float)s_2DData.TexutreSlotIndex;
			s_2DData.TextureSlots[s_2DData.TexutreSlotIndex] = texture;
			s_2DData.TexutreSlotIndex++;
		}

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::rotate(glm::mat4(1.0f), rotation_angle, { 0.0f, 0.0f, 1.0f })
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		s_2DData.QuadVertexBufferPtr->Position = transform * s_2DData.QuadVertexPositions[0];
		s_2DData.QuadVertexBufferPtr->Color = color;
		s_2DData.QuadVertexBufferPtr->TexCoord = { (x * spriteWidth) / sheetWidth, (y * spriteHeight) / sheetHeight };
		s_2DData.QuadVertexBufferPtr->TexIndex = textureIndex;
		s_2DData.QuadVertexBufferPtr->TilingFactor = tileMultiplier;
		s_2DData.QuadVertexBufferPtr++;

		s_2DData.QuadVertexBufferPtr->Position = transform * s_2DData.QuadVertexPositions[1];
		s_2DData.QuadVertexBufferPtr->Color = color;
		s_2DData.QuadVertexBufferPtr->TexCoord = { ((x+1) * spriteWidth) / sheetWidth, (y * spriteHeight) / sheetHeight };
		s_2DData.QuadVertexBufferPtr->TexIndex = textureIndex;
		s_2DData.QuadVertexBufferPtr->TilingFactor = tileMultiplier;
		s_2DData.QuadVertexBufferPtr++;

		s_2DData.QuadVertexBufferPtr->Position = transform * s_2DData.QuadVertexPositions[2];
		s_2DData.QuadVertexBufferPtr->Color = color;
		s_2DData.QuadVertexBufferPtr->TexCoord = { ((x+1) * spriteWidth) / sheetWidth, ((y+1) * spriteHeight) / sheetHeight };
		s_2DData.QuadVertexBufferPtr->TexIndex = textureIndex;
		s_2DData.QuadVertexBufferPtr->TilingFactor = tileMultiplier;
		s_2DData.QuadVertexBufferPtr++;

		s_2DData.QuadVertexBufferPtr->Position = transform * s_2DData.QuadVertexPositions[3];
		s_2DData.QuadVertexBufferPtr->Color = color;
		s_2DData.QuadVertexBufferPtr->TexCoord = { (x * spriteWidth) / sheetWidth, ((y+1) * spriteHeight) / sheetHeight };
		s_2DData.QuadVertexBufferPtr->TexIndex = textureIndex;
		s_2DData.QuadVertexBufferPtr->TilingFactor = tileMultiplier;
		s_2DData.QuadVertexBufferPtr++;

		s_2DData.QuadIndexCount += 6; // Add 6 more indices

		// Increment stats
		s_2DData.Stats.QuadCount++;
	}

	Renderer2D::Statistics Renderer2D::GetStats()
	{
		return s_2DData.Stats;
	}

	void Renderer2D::ResetStats()
	{
		memset(&s_2DData.Stats, 0, sizeof(Statistics));
	}

}

