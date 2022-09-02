#pragma once

#include "oak/core/base.h"

#include <string>

namespace Oak {

	class ConsoleMessage
	{
	public:
		enum class Category
		{
			None = -1,
			Trace = 1 << 0,
			Info = 1 << 1,
			Warn = 1 << 2,
			Error = 1 << 3,
			Fatal = 1 << 4
		};

	public:
		ConsoleMessage()
			: m_MessageID(0), m_Message(""), m_Count(0), m_Category(Category::None) {}

		ConsoleMessage(const std::string& message, Category category)
			: m_MessageID(std::hash<std::string>()(message)), m_Message(message), m_Count(1), m_Category(category) {}

		uint64_t GetMessageID() const { return m_MessageID; }
		const std::string& GetMessage() const { return m_Message; }
		uint32_t GetCount() const { return m_Count; }
		Category  GetCategory() const { return m_Category; }

	private:
		uint64_t m_MessageID;
		std::string m_Message;
		uint32_t m_Count;
		Category m_Category;

		friend class EditorConsolePanel;
	};

}
