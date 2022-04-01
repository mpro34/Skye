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
	m_Texture = Skye::Texture2D::Create("assets/textures/Checkerboard.png");
}

void Sandbox2D::OnDetach()
{

}

void Sandbox2D::OnUpdate(Skye::Timestep ts)
{
	SK_PROFILE_FUNCTION();
	
	// Update
	{
		SK_PROFILE_SCOPE("Sandbox2D CameraController update");
		m_CameraController.OnUpdate(ts);
	}
	
	// Render
	{
		SK_PROFILE_SCOPE("Sandbox2D Render prep");
		Skye::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 0 });
		Skye::RenderCommand::Clear();
	}
	{
		SK_PROFILE_SCOPE("Sandbox2D Render draw");
		Skye::Renderer2D::BeginScene(m_CameraController.GetCamera());
		Skye::Renderer2D::DrawQuad({ 0.0f, 0.0f }, 0.0f, { 1.0f, 1.0f }, { 0.8f, 0.2f, 0.1f, 1.0f });
		Skye::Renderer2D::DrawQuad({ -1.0f, 0.0f }, 0.0f, { 0.5f, 1.5f }, { 0.1f, 0.2f, 0.8f, 1.0f });
		Skye::Renderer2D::DrawQuad({ 0.0f, 0.0f, -0.1f }, 0.0f, { 10.0f, 10.0f }, m_Texture);
		Skye::Renderer2D::EndScene();
	}
}

void Sandbox2D::OnImGuiRender()
{
	SK_PROFILE_FUNCTION();

	ImGui::Begin("Settings");
	ImGui::ColorEdit4("Square Color", glm::value_ptr(m_SquareColor));
	ImGui::End();
}

void Sandbox2D::OnEvent(Skye::Event& e)
{
	SK_PROFILE_FUNCTION();

	m_CameraController.OnEvent(e);
}