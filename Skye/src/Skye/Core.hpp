#pragma once

#ifdef SK_PLATFORM_WINDOWS
	#ifdef SK_BUILD_DLL
		#define SKYE_API __declspec(dllexport)
	#else
		#define SKYE_API __declspec(dllimport)
	#endif
#else
	#error Skye only supports Windows!
#endif