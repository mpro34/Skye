#pragma once

#include "Core.hpp"
#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/fmt/ostr.h"

namespace Skye {

	class SKYE_API Log
	{
	public:
		static void Init();

		inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
		inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }

	private:
		static std::shared_ptr<spdlog::logger> s_CoreLogger;
		static std::shared_ptr<spdlog::logger> s_ClientLogger;
	};
}

// Macro to pass any args to CORE / CLIENT to the appropriate function of the logger.
#define SK_CORE_TRACE(...)   ::Skye::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define SK_CORE_INFO(...)    ::Skye::Log::GetCoreLogger()->info(__VA_ARGS__)
#define SK_CORE_WARN(...)    ::Skye::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define SK_CORE_ERROR(...)   ::Skye::Log::GetCoreLogger()->error(__VA_ARGS__)
#define SK_CORE_FATAL(...)   ::Skye::Log::GetCoreLogger()->fatal(__VA_ARGS__)

#define SK_TRACE(...)        ::Skye::Log::GetClientLogger()->trace(__VA_ARGS__)
#define SK_INFO(...)         ::Skye::Log::GetClientLogger()->info(__VA_ARGS__)
#define SK_WARN(...)         ::Skye::Log::GetClientLogger()->warn(__VA_ARGS__)
#define SK_ERROR(...)        ::Skye::Log::GetClientLogger()->error(__VA_ARGS__)
#define SK_FATAL(...)        ::Skye::Log::GetClientLogger()->fatal(__VA_ARGS__)