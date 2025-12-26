#pragma once
#include "Core.h"
#include "spdlog/spdlog.h"

namespace LazyDog{

	class LazyDog_API Log
	{
	public:
		static void Init();

		inline static std::shared_ptr<spdlog::logger> GetCoreLogger() {return s_CoreLogger; }
		inline static std::shared_ptr<spdlog::logger> GetClientLogger() {return s_ClientLogger; }

	private:
		static std::shared_ptr<spdlog::logger> s_CoreLogger;
		static std::shared_ptr<spdlog::logger> s_ClientLogger;
	};
}

//Core log macros
#define LD_CORE_TRACE(...)	::LazyDog::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define LD_CORE_INFO(...)	::LazyDog::Log::GetCoreLogger()->info(__VA_ARGS__)
#define LD_CORE_WARN(...)	::LazyDog::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define LD_CORE_ERROR(...)	::LazyDog::Log::GetCoreLogger()->error(__VA_ARGS__)
#define LD_CORE_FATAL(...)	::LazyDog::Log::GetCoreLogger()->fatal(__VA_ARGS__)

//Client log macros
#define LD_TRACE(...)		::LazyDog::Log::GetClientLogger()->trace(__VA_ARGS__)
#define LD_INFO(...)		::LazyDog::Log::GetClientLogger()->info(__VA_ARGS__)
#define LD_WARN(...)		::LazyDog::Log::GetClientLogger()->warn(__VA_ARGS__)
#define LD_ERROR(...)		::LazyDog::Log::GetClientLogger()->error(__VA_ARGS__)
#define LD_FATAL(...)		::LazyDog::Log::GetClientLogger()->fatal(__VA_ARGS__)
