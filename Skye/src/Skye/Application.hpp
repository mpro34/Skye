#pragma once

#include "Core.hpp"
#include "Events/Event.hpp"

namespace Skye {

	class SKYE_API Application
	{
	public:
		Application();
		virtual ~Application();

		void run();
	};

	// To be defined in client (sandbox)
	Application* CreateApplication();
}
