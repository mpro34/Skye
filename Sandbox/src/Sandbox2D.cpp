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

	m_GrassTex = Skye::SubTexture2D::CreateFromCoords(m_SpriteSheet,	  { 1, 2 });
	m_DirtTex = Skye::SubTexture2D::CreateFromCoords(m_SpriteSheet,		  { 6, 2 });
	m_CannonTex = Skye::SubTexture2D::CreateFromCoords(m_SpriteSheet,	  { 19, 2 });
	m_SoldierTex = Skye::SubTexture2D::CreateFromCoords(m_SpriteSheet,	  { 16, 2 });
	m_BushTex1 = Skye::SubTexture2D::CreateFromCoords(m_SpriteSheet,	  { 16, 7 });
	m_BushTex2 = Skye::SubTexture2D::CreateFromCoords(m_SpriteSheet,	  { 19, 7 });
	m_CannonFireTex = Skye::SubTexture2D::CreateFromCoords(m_SpriteSheet, { 19, 0 });

	// Selectable units
	m_SoldierTex2 = Skye::SubTexture2D::CreateFromCoords(m_SpriteSheet, { 15, 2 }); 
	m_SoldierTex3 = Skye::SubTexture2D::CreateFromCoords(m_SpriteSheet, { 17, 2 });
	m_PlaneTex1 = Skye::SubTexture2D::CreateFromCoords(m_SpriteSheet,	{ 17, 1 });
	m_PlaneTex2 = Skye::SubTexture2D::CreateFromCoords(m_SpriteSheet,	{ 18, 1 });
	m_CannonTex2 = Skye::SubTexture2D::CreateFromCoords(m_SpriteSheet,	{ 20, 2 });
	m_CannonTex3 = Skye::SubTexture2D::CreateFromCoords(m_SpriteSheet,	{ 20, 4 });

	m_ChosenTex = m_SoldierTex;

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

	Skye::Renderer2D::DrawQuad({ 0.0f, -2.0f, 0.2f }, { 1.0f, 1.0f }, glm::radians(0.0f), m_GrassTex);
	Skye::Renderer2D::DrawQuad({ 0.0f, -1.0f, 0.2f }, { 1.0f, 1.0f }, glm::radians(0.0f), m_GrassTex);
	Skye::Renderer2D::DrawQuad({ 0.0f, 0.0f, 0.2f }, { 1.0f, 1.0f }, glm::radians(0.0f), m_GrassTex);
	Skye::Renderer2D::DrawQuad({ 0.0f, 1.0f, 0.2f }, { 1.0f, 1.0f }, glm::radians(0.0f), m_GrassTex);
	Skye::Renderer2D::DrawQuad({ 0.0f, 2.0f, 0.2f }, { 1.0f, 1.0f }, glm::radians(0.0f), m_GrassTex);

	Skye::Renderer2D::DrawQuad({ 1.0f, 2.0f, 0.2f }, { 1.0f, 1.0f }, glm::radians(0.0f), m_GrassTex);
	Skye::Renderer2D::DrawQuad({ 1.0f, 2.0f, 0.3f }, { 1.0f, 1.0f }, glm::radians(0.0f), m_BushTex1);
	
	Skye::Renderer2D::DrawQuad({ 1.0f, -2.0f, 0.2f }, { 1.0f, 1.0f }, glm::radians(0.0f), m_DirtTex);
	Skye::Renderer2D::DrawQuad({ 1.0f, -1.0f, 0.2f }, { 1.0f, 1.0f }, glm::radians(0.0f), m_DirtTex);
	Skye::Renderer2D::DrawQuad({ 1.0f, 0.0f, 0.2f }, { 1.0f, 1.0f }, glm::radians(0.0f), m_DirtTex);
	Skye::Renderer2D::DrawQuad({ 1.0f, 1.0f, 0.2f }, { 1.0f, 1.0f }, glm::radians(0.0f), m_DirtTex);
	Skye::Renderer2D::DrawQuad({ 1.0f, 2.0f, 0.2f }, { 1.0f, 1.0f }, glm::radians(0.0f), m_DirtTex);

	Skye::Renderer2D::DrawQuad({ 2.0f, -2.0f, 0.2f }, { 1.0f, 1.0f }, glm::radians(0.0f), m_GrassTex);
	Skye::Renderer2D::DrawQuad({ 2.0f, -1.0f, 0.2f }, { 1.0f, 1.0f }, glm::radians(0.0f), m_GrassTex);
	Skye::Renderer2D::DrawQuad({ 2.0f, 0.0f, 0.2f }, { 1.0f, 1.0f }, glm::radians(0.0f), m_DirtTex);
	Skye::Renderer2D::DrawQuad({ 2.0f, 1.0f, 0.2f }, { 1.0f, 1.0f }, glm::radians(0.0f), m_DirtTex);
	Skye::Renderer2D::DrawQuad({ 2.0f, 2.0f, 0.2f }, { 1.0f, 1.0f }, glm::radians(0.0f), m_DirtTex);

	Skye::Renderer2D::DrawQuad({ 1.0f, 1.0f, 0.3f }, { 1.0f, 1.0f }, glm::radians(0.0f), m_SoldierTex);
	Skye::Renderer2D::DrawQuad({ 2.0f, 0.5f, 0.3f }, { 1.0f, 1.0f }, glm::radians(0.0f), m_SoldierTex);

	Skye::Renderer2D::DrawQuad({ 2.0f, -1.0f, 0.3f }, { 1.0f, 1.0f }, glm::radians(0.0f), m_CannonTex);
	Skye::Renderer2D::DrawQuad({ 2.0f, -0.25f, 0.3f }, { 1.0f, 1.0f }, glm::radians(0.0f), m_CannonFireTex);

	Skye::Renderer2D::DrawQuad({ 3.0f, -2.0f, 0.2f }, { 1.0f, 1.0f }, glm::radians(0.0f), m_GrassTex);
	Skye::Renderer2D::DrawQuad({ 3.0f, -1.0f, 0.2f }, { 1.0f, 1.0f }, glm::radians(0.0f), m_GrassTex);
	Skye::Renderer2D::DrawQuad({ 3.0f, 0.0f, 0.2f }, { 1.0f, 1.0f }, glm::radians(0.0f), m_GrassTex);
	Skye::Renderer2D::DrawQuad({ 3.0f, 1.0f, 0.2f }, { 1.0f, 1.0f }, glm::radians(0.0f), m_GrassTex);
	Skye::Renderer2D::DrawQuad({ 3.0f, 2.0f, 0.2f }, { 1.0f, 1.0f }, glm::radians(0.0f), m_DirtTex);
	Skye::Renderer2D::DrawQuad({ 3.0f, 1.0f, 0.3f }, { 1.0f, 1.0f }, glm::radians(0.0f), m_BushTex2);


	Skye::Renderer2D::DrawQuad({ 4.0f, -2.0f, 0.2f }, { 1.0f, 1.0f }, glm::radians(0.0f), m_GrassTex);
	Skye::Renderer2D::DrawQuad({ 4.0f, -1.0f, 0.2f }, { 1.0f, 1.0f }, glm::radians(0.0f), m_GrassTex);
	Skye::Renderer2D::DrawQuad({ 4.0f, 0.0f, 0.2f }, { 1.0f, 1.0f }, glm::radians(0.0f), m_GrassTex);
	Skye::Renderer2D::DrawQuad({ 4.0f, 1.0f, 0.2f }, { 1.0f, 1.0f }, glm::radians(0.0f), m_GrassTex);
	Skye::Renderer2D::DrawQuad({ 4.0f, 2.0f, 0.2f }, { 1.0f, 1.0f }, glm::radians(0.0f), m_DirtTex);


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
		//m_Particle.Position = { x + pos.x, y + pos.y };
		//for (int i = 0; i < 5; i++)
		//	m_ParticleSystem.Emit(m_Particle);

		auto posx = x + pos.x;
		auto posy = y + pos.y;

		//Skye::Renderer2D::DrawQuad({ 0.0f, 0.0f, 0.2f }, { x, y }, glm::radians(0.0f), m_TexturePlane);
		Skye::Renderer2D::DrawQuad({ posx, posy, 0.3f }, { 1.0f, 1.0f }, glm::radians(0.0f), m_ChosenTex);
	}

	Skye::Renderer2D::EndScene();


	//m_ParticleSystem.OnUpdate(ts);
	//m_ParticleSystem.OnRender(m_CameraController.GetCamera());
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

	const char* items[] = { "Soldier", "Soldier2", "Soldier3", "Plane1", "Plane2", "Cannon", "Cannon2", "Cannon3"};
	static const char* current_item = "Soldier";

	if (ImGui::BeginCombo("##combo", current_item)) // The second parameter is the label previewed before opening the combo.
	{
		for (int n = 0; n < IM_ARRAYSIZE(items); n++)
		{
			bool is_selected = (current_item == items[n]); // You can store your selection however you want, outside or inside your objects
			if (ImGui::Selectable(items[n], is_selected))
			{
				current_item = items[n];
				if (current_item == "Soldier")
				{
					m_ChosenTex = m_SoldierTex;
				}
				else if (current_item == "Soldier2")
				{
					m_ChosenTex = m_SoldierTex2;
				}
				else if (current_item == "Soldier3")
				{
					m_ChosenTex = m_SoldierTex3;
				}
				else if (current_item == "Plane1")
				{
					m_ChosenTex = m_PlaneTex1;
				}
				else if (current_item == "Plane2")
				{
					m_ChosenTex = m_PlaneTex2;
				}
				else if (current_item == "Cannon")
				{
					m_ChosenTex = m_CannonTex;
				}
				else if (current_item == "Cannon2")
				{
					m_ChosenTex = m_CannonTex;
				}
				else if (current_item == "Cannon3")
				{
					m_ChosenTex = m_CannonTex3;
				}
				else
				{
					m_ChosenTex = m_BushTex2;
				}
				if (is_selected) ImGui::SetItemDefaultFocus();   // You may set the initial focus when opening the combo (scrolling + for keyboard navigation support)
			}
		}
		ImGui::EndCombo();
	}

	ImGui::End();
}

void Sandbox2D::OnEvent(Skye::Event& e)
{
	SK_PROFILE_FUNCTION();

	m_CameraController.OnEvent(e);
}