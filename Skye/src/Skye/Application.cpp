#include "skpch.hpp"
#include "Application.hpp"

#include "Skye/Events/ApplicationEvent.hpp"
#include "Skye/Events/MouseEvent.hpp"
#include "Skye/Log.hpp"

namespace Skye {

	Application::Application()
	{
	}

	Application::~Application()
	{
	}

	void Application::run()
	{
		WindowResizeEvent e(1200, 720);

		MouseMovedEvent m(24.0, 36.0);
		SK_TRACE(m.ToString());

		SK_TRACE(e);

		while (true);
	}
}