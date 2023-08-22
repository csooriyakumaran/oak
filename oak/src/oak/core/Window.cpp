#include "oakpch.h"
#include "oak/core/Base.h"
#include "oak/core/window.h"

#ifdef WI_PLATFORM_WINDOWS
    #include "oak/platform/Windows/WindowsWindow.h"
#endif


namespace Oak
{
	// moved this to WindowsWindow
	//Window* Window::Create(const WindowSpecification& props)
	//{
	//#ifdef WI_PLATFORM_WINDOWS
	//	return new WindowsWindow(props);
	//#else
	//	LOG_CORE_CRITICAL("Unknown platform!");
	//	return nullptr;
	//#endif
	//}

}