#pragma once

#include <Skye.hpp>
#include "ParticleSystem.hpp"

class Sandbox2D : public Skye::Layer
{
public:
	Sandbox2D();
	virtual ~Sandbox2D() = default;

	virtual void OnAttach() override;
	virtual void OnDetach() override;
	virtual void OnUpdate(Skye::Timestep ts) override;
	virtual void OnImGuiRender() override;
	virtual void OnEvent(Skye::Event& e) override;
private:
	Skye::OrthographicCameraController m_CameraController;

	// Temp - Should be handled by this class!
	Skye::Ref<Skye::VertexArray> m_VertexArray;
	Skye::Ref<Skye::Shader> m_FlatColorShader;
	Skye::Ref<Skye::Texture2D> m_Texture;
	Skye::Ref<Skye::Texture2D> m_SpriteSheet;
	// Subtextures from the spritesheet
	Skye::Ref<Skye::SubTexture2D> m_GrassTex, m_DirtTex, m_CannonTex, m_SoldierTex, m_BushTex1, m_BushTex2, m_CannonFireTex;
	Skye::Ref<Skye::SubTexture2D> m_SoldierTex2, m_SoldierTex3, m_PlaneTex1, m_PlaneTex2, m_CannonTex2, m_CannonTex3;
	Skye::Ref<Skye::SubTexture2D> m_ChosenTex;

	glm::vec4 m_SquareColor = { 0.2f, 0.3f, 0.4f, 1.0f };

	ParticleProps m_Particle;
	ParticleSystem m_ParticleSystem;
};