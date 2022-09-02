#pragma once

#ifdef OAK_PLATFORM_WINDOWS
	#define OAK_DEBUG_BREAK __debugbreak()
#else
	#define OAK_DEBUG_BREAK
#endif // OAK_PLATFORM_WINDOWS

#ifdef OAK_DEBUG
	#define OAK_ENABLE_ASSERTS
#endif

#define OAK_ENABLE_VERIFY

#ifdef OAK_ENABLE_ASSERTS
	#define OAK_CORE_ASSERT_MESSAGE_INTERNAL(...)  ::Oak::Log::PrintAssertMessage(::Oak::Log::Type::Core, "Assertion Failed", __VA_ARGS__)
	#define OAK_ASSERT_MESSAGE_INTERNAL(...)  ::Oak::Log::PrintAssertMessage(::Oak::Log::Type::Client, "Assertion Failed", __VA_ARGS__)
	
	#define OAK_CORE_ASSERT(condition, ...) { if(!(condition)) { OAK_CORE_ASSERT_MESSAGE_INTERNAL(__VA_ARGS__); OAK_DEBUG_BREAK; } }
	#define OAK_ASSERT(condition, ...) { if(!(condition)) { OAK_ASSERT_MESSAGE_INTERNAL(__VA_ARGS__); OAK_DEBUG_BREAK; } }
#else
	#define OAK_CORE_ASSERT(condition, ...)
	#define OAK_ASSERT(condition, ...)
#endif

#ifdef OAK_ENABLE_VERIFY
	#define OAK_CORE_VERIFY_MESSAGE_INTERNAL(...)  ::Oak::Log::PrintAssertMessage(::Oak::Log::Type::Core, "Verify Failed", __VA_ARGS__)
	#define OAK_VERIFY_MESSAGE_INTERNAL(...)  ::Oak::Log::PrintAssertMessage(::Oak::Log::Type::Client, "Verify Failed", __VA_ARGS__)
	
	#define OAK_CORE_VERIFY(condition, ...) { if(!(condition)) { OAK_CORE_VERIFY_MESSAGE_INTERNAL(__VA_ARGS__); OAK_DEBUG_BREAK; } }
	#define OAK_VERIFY(condition, ...) { if(!(condition)) { OAK_VERIFY_MESSAGE_INTERNAL(__VA_ARGS__); OAK_DEBUG_BREAK; } }
#else
	#define OAK_CORE_VERIFY(condition, ...)
	#define OAK_VERIFY(condition, ...)
#endif
