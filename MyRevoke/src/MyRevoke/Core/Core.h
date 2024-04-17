#pragma once
#include <memory>

#ifdef RV_PLATFORM_WINDOWS
	
#endif //RV_PLATFORM_WINDOWS

#ifdef RV_ASSERTS_ENABLE
	#define RV_CORE_ASSERT(x, ...) {if(!(x)) {RV_ENGINE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak();}}
	#define RV_ASSERT(x, ...) {if(!(x)) {RV_EDITOR_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak();}}
#else
	#define RV_CORE_ASSERT(x, ...)
	#define RV_ASSERT(x, ...)
#endif // RV_ASSERTS_ENABLE

namespace Revoke {

	template<typename T>
	using Unique = std::unique_ptr<T>;

	template<typename T>
	using Shared = std::shared_ptr<T>;

}

#define BIT(x) (1 << x)
#define RV_BIND_EVENT_FUNK(fn) std::bind(&fn, this, std::placeholders::_1)