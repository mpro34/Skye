#include <Skye.hpp>

class ExampleLayer : public Skye::Layer
{
public:
	ExampleLayer()
		: Layer("Example")
	{}

	void OnUpdate()
	{
		//Input Polling
		if (Skye::Input::IsKeyPressed(SK_KEY_TAB))
			SK_INFO("Tab is pressed (poll)!");
	}

	void OnEvent(Skye::Event& event) override
	{
		// Input handling via Events
		if (event.GetEventType() == Skye::EventType::KeyPressed)
		{
			Skye::KeyPressedEvent& e = (Skye::KeyPressedEvent&)event;
			if (e.GetKeyCode() == SK_KEY_TAB)
				SK_TRACE("Tab is pressed (event)!");
			SK_TRACE("{0}", (char)e.GetKeyCode()); // Cast keycode int (ascii) to char
		}
	}
};

class Sandbox : public Skye::Application
{
public:
	Sandbox()
	{
		// Create layer structure for application.
		PushLayer(new ExampleLayer());
		PushOverlay(new Skye::ImGuiLayer());
	}

	~Sandbox()
	{
	}
};

Skye::Application* Skye::CreateApplication()
{
	return new Sandbox();
}