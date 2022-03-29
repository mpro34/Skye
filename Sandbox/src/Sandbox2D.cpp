#include "Sandbox2D.hpp"

#include "imgui/imgui.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

Sandbox2D::Sandbox2D()
	: Layer("Sandbox2D"), m_CameraController(1280.0f / 720.0f)
{
}

void Sandbox2D::OnAttach()
{

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
	
	Skye::Renderer2D::BeginScene(m_CameraController.GetCamera());
	Skye::Renderer2D::DrawQuad({ 0.0f, 0.0f }, 0.0f, { 1.0f, 1.0f }, {0.8f, 0.2f, 0.1f, 1.0f});
	Skye::Renderer2D::DrawQuad({ -1.0f, 0.0f }, 45.0f, { 0.5f, 0.5f }, { 0.1f, 0.2f, 0.8f, 1.0f });
	Skye::Renderer2D::EndScene();
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