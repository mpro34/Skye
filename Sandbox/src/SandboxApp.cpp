#include <Skye.h>

class Sandbox : public Skye::Application
{
public:
	Sandbox()
	{}

	~Sandbox()
	{
	}
};

Skye::Application* Skye::CreateApplication()
{
	return new Sandbox();
}