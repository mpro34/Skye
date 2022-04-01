#pragma once

#include "Skye/Core/Core.hpp"

#ifdef SK_PLATFORM_WINDOWS

extern Skye::Application* Skye::CreateApplication();

int main(int argc, char** argv)
{
	Skye::Log::Init();

	SK_PROFILE_BEGIN_SESSION("Startup", "SkyeProfile-Startup.json");
	auto app = Skye::CreateApplication();
	SK_PROFILE_END_SESSION();
	
	SK_PROFILE_BEGIN_SESSION("Runtime", "SkyeProfile-Runtime.json");
	app->run();
	SK_PROFILE_END_SESSION();

	SK_PROFILE_BEGIN_SESSION("Shutdown", "SkyeProfile-Shutdown.json");
	delete app;
	SK_PROFILE_END_SESSION();
}

#endif