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
	};

	struct Renderer2DData
	{
		// Max for any given draw call
		const uint32_t MAX_QUADS = 10000;
		const uint32_t MAX_VERTICES = MAX_QUADS * 4;
		const uint32_t MAX_INDICES = MAX_QUADS * 6;

		Ref<VertexArray> QuadVertexArray;
		Ref<VertexBuffer> QuadVertexBuffer;
		Ref<Shader> TextureShader;
		Ref<Texture2D> WhiteTexture;

		uint32_t QuadIndexCount = 0;
		QuadVertex* QuadVertexBufferBase = nullptr;
		QuadVertex* QuadVertexBufferPtr = nullptr;
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
			{ShaderDataType::Float2, "a_TexCoord"}
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

		// Load shader from file
		s_2DData.TextureShader = Shader::Create("assets/shaders/Texture.glsl");
		s_2DData.TextureShader->Bind();
		s_2DData.TextureShader->SetInt("u_Texture", 0);
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
		RenderCommand::DrawIndexed(s_2DData.QuadVertexArray, s_2DData.QuadIndexCount);
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const float rotation_angle, const glm::vec2& size, const glm::vec4& color)
	{
		DrawQuad({ position.x, position.y, 0.0f }, rotation_angle, size, color);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const float rotation_angle, const glm::vec2& size, const glm::vec4& color)
	{
		SK_PROFILE_FUNCTION();

		s_2DData.QuadVertexBufferPtr->Position = position;
		s_2DData.QuadVertexBufferPtr->Color = color;
		s_2DData.QuadVertexBufferPtr->TexCoord = {0.0f, 0.0f};
		s_2DData.QuadVertexBufferPtr++;

		s_2DData.QuadVertexBufferPtr->Position = { position.x + size.x, position.y, 0.0f };
		s_2DData.QuadVertexBufferPtr->Color = color;
		s_2DData.QuadVertexBufferPtr->TexCoord = { 1.0f, 0.0f };
		s_2DData.QuadVertexBufferPtr++;

		s_2DData.QuadVertexBufferPtr->Position = { position.x + size.x, position.y + size.y, 0.0f };
		s_2DData.QuadVertexBufferPtr->Color = color;
		s_2DData.QuadVertexBufferPtr->TexCoord = { 0.0f, 1.0f };
		s_2DData.QuadVertexBufferPtr++;

		s_2DData.QuadVertexBufferPtr->Position = { position.x, position.y + size.y, 0.0f };
		s_2DData.QuadVertexBufferPtr->Color = color;
		s_2DData.QuadVertexBufferPtr->TexCoord = { 1.0f, 1.0f };
		s_2DData.QuadVertexBufferPtr++;

		s_2DData.QuadIndexCount += 6; // Add 6 more indices	

		//s_2DData.TextureShader->SetFloat("u_TileMultiplier", 1.0f);
		//s_2DData.WhiteTexture->Bind();

		//glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) *
		//	glm::rotate(glm::mat4(1.0f), glm::radians(rotation_angle), {0.0f, 0.0f, 1.0f}) *
		//	glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
		//s_2DData.TextureShader->SetMat4("u_Transform", transform);

		//s_2DData.QuadVertexArray->Bind();
		//RenderCommand::DrawIndexed(s_2DData.QuadVertexArray);
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const float rotation_angle, const glm::vec2& size, const Ref<Texture2D>& texture, float tileMultiplier, const glm::vec4& tintColor)
	{
		DrawQuad({ position.x, position.y, 0.0f }, rotation_angle, size, texture, tileMultiplier, tintColor);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const float rotation_angle, const glm::vec2& size, const Ref<Texture2D>& texture, float tileMultiplier, const glm::vec4& tintColor)
	{
		SK_PROFILE_FUNCTION();

		s_2DData.TextureShader->SetFloat4("u_Color", tintColor);
		s_2DData.TextureShader->SetFloat("u_TileMultiplier", tileMultiplier);
		texture->Bind();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) *
			glm::rotate(glm::mat4(1.0f), glm::radians(rotation_angle), { 0.0f, 0.0f, 1.0f }) *
			glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
		s_2DData.TextureShader->SetMat4("u_Transform", transform);

		s_2DData.QuadVertexArray->Bind();
		RenderCommand::DrawIndexed(s_2DData.QuadVertexArray);
	}
}
