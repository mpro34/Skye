#pragma once

#include "Camera.hpp"
#include "Texture.hpp"

namespace Skye {

	class Renderer2D
	{
	public:
		static void Init();
		static void Shutdown();
		static void Flush();

		static void BeginScene(const OrthographicCamera& camera);
		static void EndScene();

		//  Primitives - rotation in radians!
		static void DrawQuad(const glm::vec2& position, const float rotation_angle, const glm::vec2& size, const glm::vec4& color);
		static void DrawQuad(const glm::vec3& position, const float rotation_angle, const glm::vec2& size, const glm::vec4& color);
		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const float rotation_angle, const Ref<Texture2D>& texture, float tileMultiplier = 1.0f, const glm::vec4& tintColor = glm::vec4(1.0f));
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const float rotation_angle, const Ref<Texture2D>& texture, float tileMultiplier = 1.0f, const glm::vec4& tintColor = glm::vec4(1.0f));
		
		// Statistics for drawing
		struct Statistics
		{
			uint32_t DrawCalls{ 0 };
			uint32_t QuadCount{ 0 };

			uint32_t GetTotalVertexCount() { return QuadCount * 4; }
			uint32_t GetTotalIndexCount() { return QuadCount * 6; }
		};
		static Statistics GetStats();
		static void ResetStats();
	private:
		static void FlushAndReset();
	};
}