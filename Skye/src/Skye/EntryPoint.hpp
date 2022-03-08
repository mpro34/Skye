#pragma once

#ifdef SK_PLATFORM_WINDOWS

extern Skye::Application* Skye::CreateApplication();

int main(int argc, char** argv)
{
	Skye::Log::Init();
	SK_CORE_ERROR("Init core logger!");
	int a = 5;
	SK_INFO("Hello from client logger = {0}", a);

	auto app = Skye::CreateApplication();
	app->run();
	delete app;
}

#endif