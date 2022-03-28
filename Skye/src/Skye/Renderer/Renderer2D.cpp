#include "skpch.hpp"
#include "Renderer2D.hpp"

#include "VertexArray.hpp"
#include "Shader.hpp"
#include "Platform/OpenGL/OpenGLShader.hpp"
#include "RenderCommand.hpp"

namespace Skye {

	struct Renderer2DStorage
	{
		Ref<VertexArray> QuadVertexArray;
		Ref<Shader> FlatColorShader;
	};

	static Renderer2DStorage* s_2DData;

	void Renderer2D::Init()
	{
		// Make pointer to static struct to ensure vertex array and shader data destructors are called.
		s_2DData = new Renderer2DStorage();
		s_2DData->QuadVertexArray = VertexArray::Create();

		float vertices2[3 * 4] = {
			-0.5f, -0.5f, 0.0f,
			0.5f, -0.5f, 0.0f,
			0.5f, 0.5f, 0.0f,
			-0.5f, 0.5f, 0.0f,
		};
		Ref<VertexBuffer> squareVB;
		squareVB.reset(VertexBuffer::Create(vertices2, sizeof(vertices2)));
		squareVB->SetLayout({
			{ShaderDataType::Float3, "a_Position"}
		});
		s_2DData->QuadVertexArray->AddVertexBuffer(squareVB);
		uint32_t indices2[6] = { 0, 1, 2, 2, 3, 0 };
		Ref<IndexBuffer> indexBuffer2;
		indexBuffer2.reset(IndexBuffer::Create(indices2, sizeof(indices2) / sizeof(uint32_t)));
		s_2DData->QuadVertexArray->SetIndexBuffer(indexBuffer2);

		// Load shader from file
		s_2DData->FlatColorShader = Shader::Create("assets/shaders/FlatColor.glsl");
	}
	
	void Renderer2D::Shutdown()
	{
		delete s_2DData;
	}

	void Renderer2D::BeginScene(const OrthographicCamera& camera)
	{
		std::dynamic_pointer_cast<OpenGLShader>(s_2DData->FlatColorShader)->Bind();
		std::dynamic_pointer_cast<OpenGLShader>(s_2DData->FlatColorShader)->UploadUniformMat4("u_ViewProjection", camera.GetViewProjectionMatrix());
		std::dynamic_pointer_cast<OpenGLShader>(s_2DData->FlatColorShader)->UploadUniformMat4("u_Transform", glm::mat4(1.0f));
	}

	void Renderer2D::EndScene()
	{

	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
	{
		DrawQuad({ position.x, position.y, 0.0f }, size, color);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
	{
		std::dynamic_pointer_cast<OpenGLShader>(s_2DData->FlatColorShader)->Bind();
		std::dynamic_pointer_cast<Skye::OpenGLShader>(s_2DData->FlatColorShader)->UploadUniformFloat4("u_Color", color);

		s_2DData->QuadVertexArray->Bind();
		RenderCommand::DrawIndexed(s_2DData->QuadVertexArray);
	}
}
