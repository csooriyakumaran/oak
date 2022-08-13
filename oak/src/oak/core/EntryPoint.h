#pragma once

#include "oak/core/Application.h"

#ifdef OAK_PLATFORM_WINDOWS

extern Oak::Application* Oak::CreateApplication(int argc, char** argv);
// bool g_ApplicationRunning = true;
namespace Oak {
	
	int Main(int argc, char** argv)
	{
		Oak::Application* app = Oak::CreateApplication(argc, argv);
		app->Run();
		delete app;

		return 0;
	}

}


#ifdef WL_DIST

#include <Windows.h>

int APIENTRY WinMain(HINSTANCE hInst, HINSTANCE hInstPrev, PSTR cmdline, int cmdshow)
{
	return Oak::Main(__argc, __argv);
}

#else

int main(int argc, char** argv)
{
	return Oak::Main(argc, argv);
}

#endif // WL_DIST

#endif // OAK_PLATFORM_WINDOWS
