#pragma once

#include "Core.hpp"
#include "Window.hpp"
#include "Skye/LayerStack.hpp"
#include "Skye/Events/Event.hpp"
#include "Skye/Events/ApplicationEvent.hpp"

#include "Skye/ImGui/ImGuiLayer.hpp"

#include "Skye/Renderer/Shader.hpp"
#include "Skye/Renderer/Buffer.hpp"
#include "Skye/Renderer/VertexArray.hpp"

namespace Skye {

	class SKYE_API Application
	{
	public:
		Application();
		virtual ~Application();

		void run();

		void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);

		inline Window& GetWindow() { return *m_Window; }

		inline static Application& Get() { return *s_Instance; }
	private:
		bool OnWindowClosed(WindowCloseEvent& e);

		std::unique_ptr<Window> m_Window;
		ImGuiLayer* m_ImGuiLayer;
		bool m_Running = true;
		LayerStack m_LayerStack;

		// Triangle
		std::shared_ptr<Shader> m_Shader;
		std::shared_ptr<VertexArray> m_TriangleVA;

		// Square
		std::shared_ptr<Shader> m_BlueShader;
		std::shared_ptr<VertexArray> m_SquareVA;

	private:
		static Application* s_Instance;
	};

	// To be defined in client (sandbox)
	Application* CreateApplication();
}
