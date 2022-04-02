#pragma once

#include "Camera.hpp"
#include "Texture.hpp"

namespace Skye {

	class Renderer2D
	{
	public:
		static void Init();
		static void Shutdown();

		static void BeginScene(const OrthographicCamera& camera);
		static void EndScene();

		//  Primitives - rotation in degrees
		static void DrawQuad(const glm::vec2& position, const float rotation_angle, const glm::vec2& size, const glm::vec4& color);
		static void DrawQuad(const glm::vec3& position, const float rotation_angle, const glm::vec2& size, const glm::vec4& color);
		static void DrawQuad(const glm::vec2& position, const float rotation_angle, const glm::vec2& size, const Ref<Texture2D>& texture, float tileMultiplier = 1.0f, const glm::vec4& tintColor = glm::vec4(1.0f));
		static void DrawQuad(const glm::vec3& position, const float rotation_angle, const glm::vec2& size, const Ref<Texture2D>& texture, float tileMultiplier = 1.0f, const glm::vec4& tintColor = glm::vec4(1.0f));
	};
}