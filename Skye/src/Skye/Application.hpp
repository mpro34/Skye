#pragma once

#include "Core.hpp"
#include "Events/Event.hpp"
#include "Skye/Events/ApplicationEvent.hpp"

#include "Window.hpp"

namespace Skye {

	class SKYE_API Application
	{
	public:
		Application();
		virtual ~Application();

		void run();

		void OnEvent(Event& e);
	private:
		bool OnWindowClosed(WindowCloseEvent& e);

		std::unique_ptr<Window> m_Window;
		bool m_Running = true;
	};

	// To be defined in client (sandbox)
	Application* CreateApplication();
}
