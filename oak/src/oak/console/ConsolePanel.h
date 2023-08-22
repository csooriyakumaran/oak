#pragma once

#include "oak/UI/Panel.h"
#include "oak/console/ConsoleMessage.h"

namespace Oak
{
	class ConsolePanel : public Panel
	{
	public:
		
		ConsolePanel();
		~ConsolePanel();

		virtual void OnEvent(Event& e) override;
		virtual void OnUIRender(const char* name, bool& open) override;

		virtual void PushLocalStyle() override;

	private:
		void RenderMenu();
		void RenderConsole();
		void RenderSearch();
		static void PushMessage(const ConsoleMessage& message);

	private:
		bool m_CollapseMessages = true;
		bool m_NewMessageAdded = false;
		bool m_ShowSearchBar = false;
		bool m_AutoScroll = true;
		bool m_DisplayMessageInspector = true;
		bool m_IsMessageInspectorHovered = false;
		bool m_IsMenuHovered = false;
		bool m_IsContextOpen = false;
		bool m_ShowFilterButtons = false;
		static constexpr uint32_t s_MessageBufferCapacity = 500;

		std::array<ConsoleMessage, s_MessageBufferCapacity> m_MessageBuffer;
		uint32_t m_MessageBufferBegin = 0;
		int32_t m_MessageFilters = (int16_t)ConsoleMessage::Category::Trace | (int16_t)ConsoleMessage::Category::Info | (int16_t)ConsoleMessage::Category::Warn| (int16_t)ConsoleMessage::Category::Error | (int16_t)ConsoleMessage::Category::Fatal;
		char m_TextFilter[128] = "";

		//ConsoleMessage* m_SelectedMessage = nullptr;
		std::map<uint32_t, ConsoleMessage*> m_SelectedMessages;
		static const char* LevelToString(ConsoleMessage::Category level)
		{
			switch (level)
			{
			case ConsoleMessage::Category::Trace: return "Trace";
			case ConsoleMessage::Category::Debug: return "Debug";
			case ConsoleMessage::Category::Info:  return "Info";
			case ConsoleMessage::Category::Warn:  return "Warn";
			case ConsoleMessage::Category::Error: return "Error";
			case ConsoleMessage::Category::Fatal: return "Fatal";
			case ConsoleMessage::Category::None: return "None";
			}
			return "";
		}
	private:
		friend class EmbeddedConsoleSink;
	};

}