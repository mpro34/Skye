#include <Skye.hpp>

class ExampleLayer : public Skye::Layer
{
public:
	ExampleLayer()
		: Layer("Example")
	{}

	void OnUpdate()
	{
		//SK_INFO("ExampleLayer::Update");
	}

	void OnEvent(Skye::Event& event) override
	{
		SK_TRACE("Example Layer: {0}", event);
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