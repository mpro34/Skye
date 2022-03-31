#pragma once

#include <memory>

#ifdef SK_PLATFORM_WINDOWS
	#if SK_DYNAMIC_LINK
		#ifdef SK_BUILD_DLL
			#define SKYE_API __declspec(dllexport)
		#else
			#define SKYE_API __declspec(dllimport)
		#endif
	#else
		#define SKYE_API
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

#define SK_BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1)

namespace Skye {

	template<typename T>
	using Scope = std::unique_ptr<T>;
	template<typename T, typename ... Args>
	constexpr Scope<T> CreateScope(Args&& ... args)
	{
		return std::make_unique<T>(std::forward<Args>(args)...);
	}

	template<typename T>
	using Ref = std::shared_ptr<T>;
	template<typename T, typename ... Args>
	constexpr Ref<T> CreateRef(Args&& ... args)
	{
		return std::make_shared<T>(std::forward<Args>(args)...);
	}
}