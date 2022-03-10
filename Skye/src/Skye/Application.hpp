#pragma once

#include "Core.hpp"
#include "Window.hpp"
#include "Skye/LayerStack.hpp"
#include "Skye/Events/Event.hpp"
#include "Skye/Events/ApplicationEvent.hpp"



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
	private:
		bool OnWindowClosed(WindowCloseEvent& e);

		std::unique_ptr<Window> m_Window;
		bool m_Running = true;
		LayerStack m_LayerStack;
	};

	// To be defined in client (sandbox)
	Application* CreateApplication();
}
