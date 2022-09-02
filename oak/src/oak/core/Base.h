

#pragma once
#include "oakpch.h"
#include "Ref.h"

namespace Oak
{
	void InitializeEngine();
	void ShutdownEngine();
}

#ifndef OAK_PLATFORM_WINDOWS
	#error The WI OAK engine only supports windows for now
#endif 


#define OAK_BIND_EVENT_FN(fn) std::bind(&##fn, this, std::placeholders::_1)
// #define OAK_BIND_EVENT_FN(fn) [this](auto&&... args) -> decltype(auto) { return this->fn(std::forward<decltype(args)>(args)...); }

#include "oak/core/Assert.h"

namespace Oak {

	template<typename T>
	using Scope = std::unique_ptr<T>;
	template<typename T, typename ... Args>
	constexpr Scope<T> CreateScope(Args&& ... args)
	{
		return std::make_unique<T>(std::forward<Args>(args)...);
	}

	// template<typename T>
	// using Ref = std::shared_ptr<T>;
	// template<typename T, typename ... Args>
	// constexpr Ref<T> CreateRef(Args&& ... args)
	// {
	// 	return std::make_shared<T>(std::forward<Args>(args)...);
	// }

}
