#pragma once

#include "Skye/Core/Timestep.hpp"
#include "Skye/Renderer/Camera.hpp"

#include "Skye/Events/ApplicationEvent.hpp"
#include "Skye/Events/MouseEvent.hpp"

namespace Skye {

	class OrthographicCameraController
	{
	public:
		OrthographicCameraController() = default;
		OrthographicCameraController(float aspect_ratio, bool rotation = false);

		virtual ~OrthographicCameraController() = default;

		void OnUpdate(Timestep ts);
		void OnEvent(Event& e);

		OrthographicCamera& GetCamera() { return m_Camera; }
		const OrthographicCamera& GetCamera() const { return m_Camera; }
	private:
		bool OnMouseScrolled(MouseScrolledEvent& e);
		bool OnWindowResized(WindowResizeEvent& e);
	private:
		float m_AspectRatio;
		float m_ZoomLevel = 1.0f;
		OrthographicCamera m_Camera;

		bool m_Rotation;
		glm::vec3 m_CameraPosition = { 0.0f, 0.0f, 0.0f };
		float m_CameraRotation = 0.0f;
		float m_CameraTranslationSpeed = 5.0f, m_CameraRotationSpeed = 180.0f;
	};
}
