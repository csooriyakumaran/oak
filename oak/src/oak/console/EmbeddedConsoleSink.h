#pragma once
#include "oak/console/ConsoleMessage.h"
#include "oak/console/ConsolePanel.h"

#include "spdlog/spdlog.h"
#include "spdlog/sinks/base_sink.h"
#include <mutex>
#include <vector>

namespace Oak
{

	class EmbeddedConsoleSink : public spdlog::sinks::base_sink<std::mutex>
	{
	public:
		explicit EmbeddedConsoleSink(uint32_t bufferCapacity)
			: m_MessageBufferCapacity(bufferCapacity), m_MessageBuffer(bufferCapacity) {}

		virtual ~EmbeddedConsoleSink() = default;

		EmbeddedConsoleSink(const EmbeddedConsoleSink& other) = delete;
		EmbeddedConsoleSink& operator=(const EmbeddedConsoleSink& other) = delete;

	protected:
		void sink_it_(const spdlog::details::log_msg& msg) override
		{
			spdlog::memory_buf_t formatted;
			spdlog::sinks::base_sink<std::mutex>::formatter_->format(msg, formatted);
			m_MessageBuffer[m_MessageCount++] = ConsoleMessage(fmt::to_string(formatted), GetMessageCategory(msg.level));

			if (m_MessageCount == m_MessageBufferCapacity)
				flush_();
		}

		void flush_() override
		{
			for (const auto& message : m_MessageBuffer)
			{
				if (message.GetCategory() == ConsoleMessage::Category::None)
					continue;

				ConsolePanel::PushMessage(message);
			}

			m_MessageCount = 0;
		}

	private:
		static ConsoleMessage::Category GetMessageCategory(spdlog::level::level_enum level)
		{
			switch (level)
			{
			case spdlog::level::trace:
				return ConsoleMessage::Category::Trace;
			case spdlog::level::debug:
				return ConsoleMessage::Category::Debug;
			case spdlog::level::info:
				return ConsoleMessage::Category::Info;
			case spdlog::level::warn:
				return ConsoleMessage::Category::Warn;
			case spdlog::level::err:
				return ConsoleMessage::Category::Error;
			case spdlog::level::critical:
				return ConsoleMessage::Category::Fatal;
			}

			CORE_ASSERT("Invalid Message Category!");
			return ConsoleMessage::Category::None;
		}

	private:
		uint32_t m_MessageBufferCapacity;
		std::vector<ConsoleMessage> m_MessageBuffer;
		uint32_t m_MessageCount = 0;
	};

}

