#include "oakpch.h"
#include "oak/utils/PlatformUtils.h"
#include "oak/core/Application.h"

// #include <commdlg.h>
#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>


namespace Oak {

    float Time::GetTime()
	{
		return glfwGetTime();
	}

} 