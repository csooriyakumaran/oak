#include "oakpch.h"
#include "oak/core/window.h"

#ifdef OAK_PLATFORM_WINDOWS
    #include "platform/Windows/WindowsWindow.h"
#endif


namespace Oak
{
	std::unique_ptr<Window> Window::Create(const WindowProps& props)
	{
	#ifdef OAK_PLATFORM_WINDOWS
		return std::make_unique<WindowsWindow>(props);
	#else
		// HZ_CORE_ASSERT(false, "Unknown platform!");
		return nullptr;
	#endif
	}

}