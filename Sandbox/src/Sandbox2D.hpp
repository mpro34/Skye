#pragma once

#include <Skye.hpp>

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

	glm::vec4 m_SquareColor = { 0.2f, 0.3f, 0.4f, 1.0f };
};