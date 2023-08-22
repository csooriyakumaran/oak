

#pragma once


#define OAK_BUILD_VERSION "v0.1.0"

namespace Oak
{
	void InitializeEngine();
	void ShutdownEngine();
}

#ifndef WI_PLATFORM_WINDOWS
	#error The WI OAK engine only supports windows for now
#endif 


#define OAK_BIND_EVENT_FN(fn) std::bind(&##fn, this, std::placeholders::_1)
// #define OAK_BIND_EVENT_FN(fn) [this](auto&&... args) -> decltype(auto) { return this->fn(std::forward<decltype(args)>(args)...); }




