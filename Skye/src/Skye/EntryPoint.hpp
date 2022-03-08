#pragma once

#ifdef SK_PLATFORM_WINDOWS

extern Skye::Application* Skye::CreateApplication();

int main(int argc, char** argv)
{
	auto app = Skye::CreateApplication();
	app->run();
	delete app;
}

#endif