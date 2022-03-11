#pragma once

#include "Core.hpp"
#include "Window.hpp"
#include "Skye/LayerStack.hpp"
#include "Skye/Events/Event.hpp"
#include "Skye/Events/ApplicationEvent.hpp"

#include "Skye/ImGui/ImGuiLayer.hpp"


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

		unsigned int m_VertexArray, m_VertexBuffer, m_IndexBuffer;
	private:
		static Application* s_Instance;
	};

	// To be defined in client (sandbox)
	Application* CreateApplication();
}
