#pragma once

#include <memory>

#ifdef LD_PLATFORM_WINDOWS
	#if LD_DYNAMIC_LINK
		#ifdef LD_BUILD_DLL
			#define LazyDog_API __declspec(dllexport)
		#else
			#define LazyDog_API __declspec(dllimport)
		#endif
	#else
		#define LazyDog_API
	#endif
#else
	#error LazyDog only support Windows!
#endif

#ifdef LD_ENABLE_ASSERTS
	#define LD_ASSERT(x, ...){if(!(x)){LD_ERROR("Assertion Failed: {0}", __VA_ARGS__);__debugbreak();}}
	#define LD_CORE_ASSERT(x, ...){if(!(x)){LD_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__);__debugbreak();}}
#else
	#define LD_ASSERT(x, ...)
	#define LD_CORE_ASSERT(x, ...)
#endif

#define BIT(x) (1 << x)

#define LD_BIND_EVENT_FN(x) std::bind(&x, this, std::placeholders::_1)


namespace LazyDog
{
	template<typename T>
	using Scope = std::unique_ptr<T>;

	template<typename T>
	using Ref = std::shared_ptr<T>;
}