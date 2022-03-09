#pragma once

#include "Core.hpp"
#include "Events/Event.hpp"
#include "Window.hpp"

namespace Skye {

	class SKYE_API Application
	{
	public:
		Application();
		virtual ~Application();

		void run();
	private:
		std::unique_ptr<Window> m_Window;
		bool m_Running = true;
	};

	// To be defined in client (sandbox)
	Application* CreateApplication();
}
