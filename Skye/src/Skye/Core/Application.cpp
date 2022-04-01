#include "skpch.hpp"
#include "Application.hpp"

#include "Skye/Core/Log.hpp"

#include "Skye/Renderer/Renderer.hpp"
#include <GLFW/glfw3.h>

#include "Input.hpp"

namespace Skye {

#define BIND_EVENT_FN(x) std::bind(&x, this, std::placeholders::_1)

	Application* Application::s_Instance = nullptr;

	Application::Application()
	{
		SK_PROFILE_FUNCTION();

		SK_CORE_ASSERT(!s_Instance, "Application already exists!");
		s_Instance = this;

		m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window->SetEventCallback(BIND_EVENT_FN(Application::OnEvent));

		Renderer::Init();

		m_ImGuiLayer = new ImGuiLayer();
		PushOverlay(m_ImGuiLayer);
	}

	Application::~Application()
	{
		SK_PROFILE_FUNCTION();
	}

	void Application::PushLayer(Layer* layer)
	{
		SK_PROFILE_FUNCTION();

		m_LayerStack.PushLayer(layer);
		layer->OnAttach();
	}

	void Application::PushOverlay(Layer* layer)
	{
		SK_PROFILE_FUNCTION();

		m_LayerStack.PushOverlay(layer);
		layer->OnAttach();
	}

	void Application::OnEvent(Event& e)
	{
		SK_PROFILE_FUNCTION();

		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(Application::OnWindowClosed));
		dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT_FN(Application::OnWindowResized));

		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin(); )
		{
			(*--it)->OnEvent(e);
			if (e.m_Handled)
				break;
		}
	}

	void Application::run()
	{
		SK_PROFILE_FUNCTION();

		while (m_Running)
		{
			SK_PROFILE_SCOPE("Application run loop");

			float time = (float)glfwGetTime(); // TODO: Platform::GetTime()
			Timestep timestep = time - m_LastFrameTime;
			m_LastFrameTime = time;

			// Only handle updates if application is not minimized (not need to render)
			if (!m_Minimized)
			{
				{
					SK_PROFILE_SCOPE("Application::LayerStack - OnUpdate()");
					for (Layer* layer : m_LayerStack)
						layer->OnUpdate(timestep);
				}

				m_ImGuiLayer->Begin();
				{
					SK_PROFILE_SCOPE("Application::LayerStack - OnImGuiRender()");
					for (Layer* layer : m_LayerStack)
						layer->OnImGuiRender();
				}
				m_ImGuiLayer->End();

				m_Window->OnUpdate();
			}
		}
	}

	// Set application to stop running and do not propagate event to any more layers.
	bool Application::OnWindowClosed(WindowCloseEvent& e)
	{
		m_Running = false;
		return true;
	}

	// Handle window resize aspect ratio change and propagate event to other layers.
	bool Application::OnWindowResized(WindowResizeEvent& e)
	{
		SK_PROFILE_FUNCTION();

		if (e.GetWidth() == 0 || e.GetHeight() == 0)
		{
			m_Minimized = true;
			return false;
		}

		m_Minimized = false;

		Renderer::OnWindowResize(e.GetWidth(), e.GetHeight());

		return false;
	}
}