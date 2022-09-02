#include "oakpch.h"
#include "oak/core/Log.h"
//#include "oak/utilities/FileSystem.h"

#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>
#include "oak/console/EmbeddedConsoleSink.h"

#include <filesystem>

#define OAK_HAS_CONSOLE !OAK_DIST

namespace Oak {

	std::shared_ptr<spdlog::logger> Log::s_EngineLogger;
	std::shared_ptr<spdlog::logger> Log::s_ClientLogger;
	std::shared_ptr<spdlog::logger> Log::s_EmbeddedConsoleLogger;

	void Log::Init()
	{
		std::string logsDirectory = "logs";
		if (!std::filesystem::exists(logsDirectory))
			std::filesystem::create_directories(logsDirectory);

		std::vector<spdlog::sink_ptr> engineSinks;
		std::vector<spdlog::sink_ptr> embeddedSinks;
		std::vector<spdlog::sink_ptr> appSinks;

		engineSinks.emplace_back(std::make_shared<spdlog::sinks::basic_file_sink_mt>("logs/OAK.log", true));
		embeddedSinks.emplace_back(std::make_shared<spdlog::sinks::basic_file_sink_mt>("logs/APP.log", true));
		appSinks.emplace_back(std::make_shared<spdlog::sinks::basic_file_sink_mt>("logs/APP.log", true));
		
		engineSinks[0]->set_pattern("[%d-%m-%Y %H:%M:%S:%e] %n: %v");
		embeddedSinks[0]->set_pattern("[%d-%m-%Y %H:%M:%S:%e] %n: %v");
		appSinks[0]->set_pattern("[%d-%m-%Y %H:%M:%S:%e] %n: %v");

#ifdef OAK_HAS_CONSOLE
		engineSinks.emplace_back(std::make_shared<spdlog::sinks::stdout_color_sink_mt>());
		embeddedSinks.emplace_back(std::make_shared<spdlog::sinks::stdout_color_sink_mt>());
		appSinks.emplace_back(std::make_shared<spdlog::sinks::stdout_color_sink_mt>());
		
		engineSinks.emplace_back(std::make_shared<EmbeddedConsoleSink>(1));
		embeddedSinks.emplace_back(std::make_shared<EmbeddedConsoleSink>(1));
		appSinks.emplace_back(std::make_shared<EmbeddedConsoleSink>(1));
		
		engineSinks[1]->set_pattern("%^[%d-%m-%Y %H:%M:%S:%e] %n: %v%$");
		embeddedSinks[1]->set_pattern("%^[%d-%m-%Y %H:%M:%S:%e] %n: %v%$");
		appSinks[1]->set_pattern("%^[%d-%m-%Y %H:%M:%S:%e] %n: %v%$");
		
		engineSinks[2]->set_pattern("%^[%d-%m-%Y %H:%M:%S:%e] %n: %v%$");
		embeddedSinks[2]->set_pattern("%^[%d-%m-%Y %H:%M:%S:%e] %n: %v%$");
		appSinks[2]->set_pattern("%^[%d-%m-%Y %H:%M:%S:%e] %n: %v%$");

#endif
		

		s_EngineLogger = std::make_shared<spdlog::logger>("ENGINE", engineSinks.begin(), engineSinks.end());
		spdlog::register_logger(s_EngineLogger);
		s_EngineLogger->set_level(spdlog::level::trace);
		s_EngineLogger->flush_on(spdlog::level::trace);

		s_ClientLogger = std::make_shared<spdlog::logger>("CLIENT", appSinks.begin(), appSinks.end());
		spdlog::register_logger(s_ClientLogger);
		s_ClientLogger->set_level(spdlog::level::trace);
		s_ClientLogger->flush_on(spdlog::level::trace);

		s_EmbeddedConsoleLogger = std::make_shared<spdlog::logger>("CONSOLE", embeddedSinks.begin(), embeddedSinks.end());
		spdlog::register_logger(s_EmbeddedConsoleLogger);
		s_EmbeddedConsoleLogger->set_level(spdlog::level::trace);
		s_EmbeddedConsoleLogger->flush_on(spdlog::level::trace);
	}

	void Log::Shutdown()
	{
		s_EmbeddedConsoleLogger.reset();
		s_ClientLogger.reset();
		s_EngineLogger.reset();
		spdlog::drop_all();
	}
}

