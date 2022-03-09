#include "skpch.hpp"
#include "Application.hpp"

#include "Skye/Events/ApplicationEvent.hpp"
#include "Skye/Log.hpp"

#include <GLFW/glfw3.h>

namespace Skye {

	Application::Application()
	{
		m_Window = std::unique_ptr<Window>(Window::Create());
	}

	Application::~Application()
	{
	}

	void Application::run()
	{
		while (m_Running)
		{
			glClearColor(1, 1, 0, 0);
			glClear(GL_COLOR_BUFFER_BIT);
			m_Window->OnUpdate();
		}
	}
}