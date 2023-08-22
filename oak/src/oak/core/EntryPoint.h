#pragma once
#include "oakpch.h"
#include "oak/core/Base.h"
#include "oak/core/Application.h"

#ifdef WI_PLATFORM_WINDOWS

extern Oak::Application* Oak::CreateApplication(int argc, char** argv);
bool g_ApplicationRunning = true;

int Oak::Main(int argc, char** argv)
{
	while (g_ApplicationRunning)
	{
		
		Oak::InitializeEngine();
		Oak::Application* app = Oak::CreateApplication(argc, argv);
		CORE_ASSERT(app, "Client Application is NULL!");
		app->Run();
		delete app;
		Oak::ShutdownEngine();
	}
	return 0;
}


#ifdef WI_DIST

#include <Windows.h>

int APIENTRY wWinMain(HINSTANCE hInst, HINSTANCE hInstPrev, PWSTR cmdline, int cmdshow)
{
	return Oak::Main(__argc, __argv);
}

#else

int main(int argc, char** argv)
{
	return Oak::Main(argc, argv);
}

#endif // WI_DIST

#endif // WI_PLATFORM_WINDOWS
