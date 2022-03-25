#pragma once

#include "Core.hpp"
#include "Window.hpp"
#include "Skye/Core/LayerStack.hpp"
#include "Skye/Events/Event.hpp"
#include "Skye/Events/ApplicationEvent.hpp"

#include "Skye/Core/Timestep.hpp"

#include "Skye/ImGui/ImGuiLayer.hpp"

namespace Skye {

	class Application
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
		bool OnWindowResized(WindowResizeEvent& e);
	private:
		std::unique_ptr<Window> m_Window;
		ImGuiLayer* m_ImGuiLayer;
		bool m_Running = true;
		bool m_Minimized = false;
		LayerStack m_LayerStack;
		float m_LastFrameTime = 0.0f;
	private:
		static Application* s_Instance;
	};

	// To be defined in client (sandbox)
	Application* CreateApplication();
}
