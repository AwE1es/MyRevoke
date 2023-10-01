#pragma once

#include<memory>

#include "Core.h"

#include "spdlog/spdlog.h"

namespace Revoke{ 
	class RV_API Log
	{
	public:

		static void Init();

		inline static std::shared_ptr<spdlog::logger> GetCoreLogger() { return s_CoreLogger; }
		inline static std::shared_ptr<spdlog::logger> GetClientLogger() { return s_ClientLogger; }

	private:
		static std::shared_ptr<spdlog::logger> s_CoreLogger;
		static std::shared_ptr<spdlog::logger> s_ClientLogger;
	};
}

// Define macros for logging
#define RV_CORE_ERROR(...)	::Revoke::Log::GetCoreLogger()->error(__VA_ARGS__)
#define RV_CORE_WARN(...)	::Revoke::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define RV_CORE_INFO(...)	::Revoke::Log::GetCoreLogger()->info(__VA_ARGS__)
#define RV_CORE_TRACE(...)	::Revoke::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define RV_CORE_FATAL(...)	::Revoke::Log::GetCoreLogger()->fatal(__VA_ARGS__)

#define RV_ERROR(...)		::Revoke::Log::GetClientLogger()->error(__VA_ARGS__)
#define RV_WARN(...)		::Revoke::Log::GetClientLogger()->warn(__VA_ARGS__)
#define RV_INFO(...)		::Revoke::Log::GetClientLogger()->info(__VA_ARGS__)
#define RV_TRACE(...)		::Revoke::Log::GetClientLogger()->trace(__VA_ARGS__)
#define RV_FATAL(...)		::Revoke::Log::GetClientLogger()->fatal(__VA_ARGS__)
