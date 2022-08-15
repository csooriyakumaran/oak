#pragma once

#include "oak/core/Base.h"
#include "oak/core/Application.h"

#ifdef OAK_PLATFORM_WINDOWS

extern Oak::Application* Oak::CreateApplication(int argc, char** argv);

int Oak::Main(int argc, char** argv)
{
	Oak::Log::Init();

	Oak::Application* app = Oak::CreateApplication(argc, argv);
	app->Run();
	delete app;

	return 0;
}


#ifdef OAK_DIST

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

#endif // OAK_DIST

#endif // OAK_PLATFORM_WINDOWS
