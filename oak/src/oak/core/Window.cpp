#include "oakpch.h"
#include "oak/core/Base.h"
#include "oak/core/window.h"

#ifdef OAK_PLATFORM_WINDOWS
    #include "platform/Windows/WindowsWindow.h"
#endif


namespace Oak
{
	Scope<Window> Window::Create(const WindowProps& props)
	{
	#ifdef OAK_PLATFORM_WINDOWS
		return CreateScope<WindowsWindow>(props);
	#else
		OAK_CORE_CRITICAL("Unknown platform!");
		return nullptr;
	#endif
	}

}