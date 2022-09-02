#include "oakpch.h"
#include "Base.h"

#include "log.h"

#define OAK_BUILD_VERSION "v0.1.0"

namespace Oak
{

    void InitializeEngine()
    {
        Log::Init();
        OAK_CORE_TRACE("Oak Engine {}", OAK_BUILD_VERSION);
        OAK_CORE_TRACE("Initializing ..");

    }

    void ShutdownEngine()
    {
        OAK_CORE_TRACE("Shutting down...");
        Log::Shutdown();
    }

}