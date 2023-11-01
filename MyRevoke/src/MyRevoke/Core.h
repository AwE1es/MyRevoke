#pragma once

#ifdef RV_PLATFORM_WINDOWS
	#ifdef RV_BUILD_DLL
		#define RV_API __declspec(dllexport)
	#else
		#define RV_API __declspec(dllimport)
	#endif 
#endif //RV_PLATFORM_WINDOWS

#ifdef RV_ASSERTS_ENABLE
	#define RV_CORE_ASSERT(x, ...) {if(!(x)) {RV_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak();}}
	#define RV_ASSERT(x, ...) {if(!(x)) {RV_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak();}}
#else
	#define RV_CORE_ASSERT(x, ...)
	#define RV_ASSERT(x, ...)
#endif // RV_ASSERTS_ENABLE


#define BIT(x) (1 << x)
#define RV_BIND_EVENT_FUNK(fn) std::bind(&fn, this, std::placeholders::_1)