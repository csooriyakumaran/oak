

#pragma once
#include "oakpch.h"



#ifdef OAK_DEBUG
#if defined(OAK_PLATFORM_WINDOWS)
#define OAK_DEBUGBREAK() __debugbreak()
#elif defined(OAK_PLATFORM_LINUX)
#include <signal.h>
#define OAK_DEBUGBREAK() raise(SIGTRAP)
#else
#error "Platform doesn't support debugbreak yet!"
#endif
#define OAK_ENABLE_ASSERTS
#else
#define OAK_DEBUGBREAK()
#endif

#define OAK_EXPAND_MACRO(x) x
#define OAK_STRINGIFY_MACRO(x) #x

#define OAK_BIND_EVENT_FN(fn) [this](auto&&... args) -> decltype(auto) { return this->fn(std::forward<decltype(args)>(args)...); }


namespace Oak {

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

#include "oak/core/Log.h"
#include "oak/core/Assert.h"