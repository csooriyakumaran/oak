#include "oakpch.h"
#include "Base.h"
#include "oak/console/EmbeddedConsoleSink.h"


namespace Oak
{

    void InitializeEngine()
    {
        wi::Random::Init();
        wi::Log::Init();
        wi::Log::AddSink<EmbeddedConsoleSink>(2);
        LOG_CORE_TRACE("Oak Engine {}", OAK_BUILD_VERSION);
        LOG_CORE_TRACE("Initializing ..");

    }

    void ShutdownEngine()
    {
        wi::Log::Shutdown();
    }

}