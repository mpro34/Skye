#include "skpch.hpp"
#include "Renderer2D.hpp"

#include "VertexArray.hpp"
#include "Shader.hpp"
#include "RenderCommand.hpp"

#include <glm/gtc/matrix_transform.hpp>

namespace Skye {

	struct Renderer2DStorage
	{
		Ref<VertexArray> QuadVertexArray;
		Ref<Shader> TextureShader;
		Ref<Texture2D> WhiteTexture;
	};

	static Renderer2DStorage* s_2DData;

	void Renderer2D::Init()
	{
		// Make pointer to static struct to ensure vertex array and shader data destructors are called.
		s_2DData = new Renderer2DStorage();
		s_2DData->QuadVertexArray = VertexArray::Create();

		float vertices2[5 * 4] = {
			-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
			0.5f, -0.5f,  0.0f, 1.0f, 0.0f,
			0.5f, 0.5f,   0.0f, 1.0f, 1.0f,
			-0.5f, 0.5f,  0.0f,	0.0f, 1.0f
		};
		Ref<VertexBuffer> squareVB;
		squareVB.reset(VertexBuffer::Create(vertices2, sizeof(vertices2)));
		squareVB->SetLayout({
			{ShaderDataType::Float3, "a_Position"},
			{ShaderDataType::Float2, "a_TexCoord"}
		});
		s_2DData->QuadVertexArray->AddVertexBuffer(squareVB);
		uint32_t indices2[6] = { 0, 1, 2, 2, 3, 0 };
		Ref<IndexBuffer> indexBuffer2;
		indexBuffer2.reset(IndexBuffer::Create(indices2, sizeof(indices2) / sizeof(uint32_t)));
		s_2DData->QuadVertexArray->SetIndexBuffer(indexBuffer2);

		s_2DData->WhiteTexture = Texture2D::Create(1, 1);
		uint32_t whiteTextureData = 0xffffffff; // Full white texture
		s_2DData->WhiteTexture->SetData(&whiteTextureData, sizeof(whiteTextureData));

		// Load shader from file
		s_2DData->TextureShader = Shader::Create("assets/shaders/Texture.glsl");
		s_2DData->TextureShader->Bind();
		s_2DData->TextureShader->SetInt("u_Texture", 0);
	}
	
	void Renderer2D::Shutdown()
	{
		delete s_2DData;
	}

	void Renderer2D::BeginScene(const OrthographicCamera& camera)
	{
		s_2DData->TextureShader->Bind();
		s_2DData->TextureShader->SetMat4("u_ViewProjection", camera.GetViewProjectionMatrix());
	}

	void Renderer2D::EndScene()
	{

	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const float rotation_angle, const glm::vec2& size, const glm::vec4& color)
	{
		DrawQuad({ position.x, position.y, 0.0f }, rotation_angle, size, color);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const float rotation_angle, const glm::vec2& size, const glm::vec4& color)
	{
		s_2DData->TextureShader->SetFloat4("u_Color", color);
		s_2DData->WhiteTexture->Bind();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) *
			glm::rotate(glm::mat4(1.0f), glm::radians(rotation_angle), glm::vec3(1.0f)) *
			glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
		s_2DData->TextureShader->SetMat4("u_Transform", transform);

		s_2DData->QuadVertexArray->Bind();
		RenderCommand::DrawIndexed(s_2DData->QuadVertexArray);
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const float rotation_angle, const glm::vec2& size, const Ref<Texture2D>& texture)
	{
		DrawQuad({ position.x, position.y, 0.0f }, rotation_angle, size, texture);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const float rotation_angle, const glm::vec2& size, const Ref<Texture2D>& texture)
	{
		s_2DData->TextureShader->SetFloat4("u_Color", glm::vec4(1.0f));
		texture->Bind();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) *
			glm::rotate(glm::mat4(1.0f), glm::radians(rotation_angle), glm::vec3(1.0f)) *
			glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
		s_2DData->TextureShader->SetMat4("u_Transform", transform);

		s_2DData->QuadVertexArray->Bind();
		RenderCommand::DrawIndexed(s_2DData->QuadVertexArray);
	}
}
