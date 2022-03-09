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

#ifdef SK_ENABLE_ASSERTS
	#define SK_ASSERT(x, ...) { if(!x)) { SK_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
	#define SK_CORE_ASSERT(x, ...) { if(!x)) { SK_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
#else
	#define SK_ASSERT(x, ...)
	#define SK_CORE_ASSERT(x, ...)
#endif

#define BIT_SHIFT_LEFT(x) (1 << x)