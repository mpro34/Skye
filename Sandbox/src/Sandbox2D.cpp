#include "Sandbox2D.hpp"

#include "Platform/OpenGL/OpenGLShader.hpp"
#include "imgui/imgui.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

Sandbox2D::Sandbox2D()
	: Layer("Sandbox2D"), m_CameraController(1280.0f / 720.0f)
{
}

void Sandbox2D::OnAttach()
{
	// -- Create Square -- //
	m_VertexArray = Skye::VertexArray::Create();
	float vertices2[3 * 4] = {
		-0.5f, -0.5f, 0.0f,
		0.5f, -0.5f, 0.0f, 
		0.5f, 0.5f, 0.0f,
		-0.5f, 0.5f, 0.0f, 
	};
	Skye::Ref<Skye::VertexBuffer> squareVB;
	squareVB.reset(Skye::VertexBuffer::Create(vertices2, sizeof(vertices2)));
	squareVB->SetLayout({
		{Skye::ShaderDataType::Float3, "a_Position"}
	});
	m_VertexArray->AddVertexBuffer(squareVB);
	uint32_t indices2[6] = { 0, 1, 2, 2, 3, 0 };
	Skye::Ref<Skye::IndexBuffer> indexBuffer2;
	indexBuffer2.reset(Skye::IndexBuffer::Create(indices2, sizeof(indices2) / sizeof(uint32_t)));
	m_VertexArray->SetIndexBuffer(indexBuffer2);

	// Load shader from file
	m_FlatColorShader = Skye::Shader::Create("assets/shaders/FlatColor.glsl");
}

void Sandbox2D::OnDetach()
{

}

void Sandbox2D::OnUpdate(Skye::Timestep ts)
{
	// Update
	m_CameraController.OnUpdate(ts);

	// Render
	Skye::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 0 });
	Skye::RenderCommand::Clear();

	Skye::Renderer::BeginScene(m_CameraController.GetCamera());

	std::dynamic_pointer_cast<Skye::OpenGLShader>(m_FlatColorShader)->Bind();
	std::dynamic_pointer_cast<Skye::OpenGLShader>(m_FlatColorShader)->UploadUniformFloat4("u_Color", m_SquareColor);

	Skye::Renderer::Submit(m_FlatColorShader, m_VertexArray, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));

	Skye::Renderer::EndScene();
}

void Sandbox2D::OnImGuiRender()
{
	ImGui::Begin("Settings");
	ImGui::ColorEdit4("Square Color", glm::value_ptr(m_SquareColor));
	ImGui::End();
}

void Sandbox2D::OnEvent(Skye::Event& e)
{
	m_CameraController.OnEvent(e);
}