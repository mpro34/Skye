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
	m_SpriteSheet = Skye::Texture2D::Create("assets/game/textures/towerDefense_tilesheet@2.png");
	// Initialize particle props
	m_Particle.ColorBegin = { 254 / 255.0f, 212 / 255.0f, 123 / 255.0f, 1.0f };
	m_Particle.ColorEnd = { 254 / 255.0f, 109 / 255.0f, 41 / 255.0f, 1.0f };
	m_Particle.SizeBegin = 0.5f, m_Particle.SizeVariation = 0.3f, m_Particle.SizeEnd = 0.0f;
	m_Particle.LifeTime = 5.0f;
	m_Particle.Velocity = { 0.0f, 0.0f };
	m_Particle.VelocityVariation = { 3.0f, 1.0f };
	m_Particle.Position = { 0.0f, 0.0f };
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
	Skye::Renderer2D::ResetStats();
	{
		SK_PROFILE_SCOPE("Sandbox2D Render prep");
		Skye::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 0 });
		Skye::RenderCommand::Clear();
	}
#if 0
	{
		static float rotation = 0.0f;
		rotation += ts * 40.0f;

		SK_PROFILE_SCOPE("Sandbox2D Render draw");
		Skye::Renderer2D::BeginScene(m_CameraController.GetCamera());

		Skye::Renderer2D::DrawQuad({ 1.0f, 0.0f }, glm::radians(45.0f), { 1.0f, 1.0f }, { 0.8f, 0.2f, 0.1f, 1.0f }); // red square
		Skye::Renderer2D::DrawQuad({ -1.0f, 0.0f }, glm::radians(0.0f), { 0.5f, 1.5f }, { 0.1f, 0.2f, 0.8f, 1.0f }); // blue rectangle
		Skye::Renderer2D::DrawQuad({ 0.0f, 0.0f, -0.1f }, { 20.0f, 20.0f }, glm::radians(0.0f), m_Texture, 10.0f); // Background texture
		Skye::Renderer2D::DrawQuad({ 0.0f, 0.0f, 0.0f }, { 1.0f, 1.0f }, glm::radians(rotation), m_Texture, 20.0f); // Textured square

		Skye::Renderer2D::EndScene();

		Skye::Renderer2D::BeginScene(m_CameraController.GetCamera());
		for (float y = -5.0f; y < 5.0f; y += 0.5)
		{
			for (float x = -5.0f; x < 5.0f; x += 0.5)
			{
				glm::vec4 color = {(x + 5.0f) / 10.0f, 0.4f, (y + 0.5f) / 10.0f, 1.0f };
				Skye::Renderer2D::DrawQuad({ x, y }, 0.0f, { 0.45f, 0.45f }, color);
			}
		}
		Skye::Renderer2D::EndScene();
	}
#endif
	// Draw sprite sheet
	Skye::Renderer2D::BeginScene(m_CameraController.GetCamera());
	Skye::Renderer2D::DrawQuad({ 0.0f, 0.0f, 0.2f }, { 1.0f, 1.0f }, glm::radians(0.0f), m_SpriteSheet); // Background texture
	Skye::Renderer2D::EndScene();

	// Draw Particles on mouse cursor
	if (Skye::Input::IsMouseButtonPressed(SK_MOUSE_BUTTON_LEFT))
	{
		auto [x, y] = Skye::Input::GetMousePosition();
		auto width = Skye::Application::Get().GetWindow().GetWidth();
		auto height = Skye::Application::Get().GetWindow().GetHeight();

		auto bounds = m_CameraController.GetBounds();
		auto pos = m_CameraController.GetCamera().GetPosition();
		x = (x / width) * bounds.GetWidth() - bounds.GetWidth() * 0.5f;
		y = bounds.GetHeight() * 0.5f - (y / height) * bounds.GetHeight();
		m_Particle.Position = { x + pos.x, y + pos.y };
		for (int i = 0; i < 5; i++)
			m_ParticleSystem.Emit(m_Particle);
	}

	m_ParticleSystem.OnUpdate(ts);
	m_ParticleSystem.OnRender(m_CameraController.GetCamera());
}

void Sandbox2D::OnImGuiRender()
{
	SK_PROFILE_FUNCTION();

	ImGui::Begin("Settings");

	auto stats = Skye::Renderer2D::GetStats();
	ImGui::Text("Renderer2D Stats:");
	ImGui::Text("Draw Calls = %d", stats.DrawCalls);
	ImGui::Text("Quad Calls = %d", stats.QuadCount);
	ImGui::Text("Vertex Count = %d", stats.GetTotalVertexCount());
	ImGui::Text("Index Count = %d", stats.GetTotalIndexCount());

	ImGui::ColorEdit4("Square Color", glm::value_ptr(m_SquareColor));
	ImGui::End();
}

void Sandbox2D::OnEvent(Skye::Event& e)
{
	SK_PROFILE_FUNCTION();

	m_CameraController.OnEvent(e);
}