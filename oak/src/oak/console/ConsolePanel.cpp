#include "oakpch.h"
#include "ConsolePanel.h"
#include "imgui.h"

namespace Oak
{

	ImVec4 TRACE_COL = ImVec4(144.0f / 255.0f, 164.0f / 255.0f, 174.0f / 255.0f, 255.0f / 255.0f);
	ImVec4 INFO_COL  = ImVec4(102.0f/ 255.0f, 187.0f / 255.0f, 106.0f / 255.0f, 255.0f / 255.0f);
	ImVec4 WARN_COL  = ImVec4(255.0f / 255.0f, 193.0f / 255.0f, 7.0f / 255.0f, 1.0f);
	ImVec4 ERROR_COL = ImVec4(230.0f / 255.0f, 81.0f / 255.0f, 0.0f / 255.0f, 1.0f);
	ImVec4 FATAL_COL = ImVec4(183.0f / 255.0f, 28.0f / 255.0f, 28.0f / 255.0f, 1.0f);

	static ConsolePanel* s_Instance = nullptr;

	ConsolePanel::ConsolePanel()
	{
		CORE_ASSERT(s_Instance == nullptr);
		s_Instance = this;
	}

	ConsolePanel::~ConsolePanel()
	{
		s_Instance = nullptr;
	}

	void ConsolePanel::OnEvent(Event& e)
	{

	}

	void ConsolePanel::PushLocalStyle()
	{
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f)); m_StylePopCount++;
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize,0.0f); m_StylePopCount++;
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing,ImVec2(0.0f, 0.0f)); m_StylePopCount++;
	}

	void ConsolePanel::OnUIRender(const char* name, bool& open)
	{
		ImGui::Begin(name, &open);
		RenderMenu();
		ImGui::Separator();
		RenderConsole();
		if (m_ShowSearchBar)
		{
			//ImGui::Separator();
			RenderSearch();
		}
		ImGui::End();
	}

	void ConsolePanel::RenderSearch()
	{
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::BeginChild("##SEARCHBAR", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y), false, ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoScrollbar);
		ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
		ImGui::InputTextWithHint("##SEARCH", "Search ... ", m_TextFilter, IM_ARRAYSIZE(m_TextFilter));
		ImGui::EndChild();
		ImGui::PopStyleVar();
	}

	void ConsolePanel::RenderMenu()
	{
		ImGui::BeginChild("##CONSOLEMENU", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetFrameHeight()), false, ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoScrollbar);
		m_IsMenuHovered = ImGui::IsWindowHovered();
		int style_pop_count = 0;
		int color_pop_count = 0;
		
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0)); color_pop_count++;
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.3f, 0.3f, 0.3f, 1.0f)); color_pop_count++;
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0, 0, 0, 0)); color_pop_count++;
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f)); style_pop_count++;
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0)); style_pop_count++;
		ImGui::PushStyleVar(ImGuiStyleVar_ItemInnerSpacing, ImVec2(5, 0)); style_pop_count++;
		

		float buttonwidth = 65;
		ImVec2 buttonSize = ImVec2(buttonwidth, ImGui::GetFrameHeight());
		
		ImGui::SetCursorPosX(5.0f);
		if(ImGui::Button("CLEAR"))
			m_MessageBufferBegin = 0;
		
		
		ImGui::SameLine();
		if (ImGui::Button("SEARCH"))
			m_ShowSearchBar ^= true;

		// Options menu
		if (ImGui::BeginPopup("OPTIONS"))
		{
			ImGui::PushStyleVar(ImGuiStyleVar_CellPadding, ImVec2(5.0f, 5.0f));
			ImGui::Checkbox("Auto-scroll", &m_AutoScroll);
			ImGui::Checkbox("View Selected Messages", &m_DisplayMessageInspector);
			ImGui::Checkbox("Collapse Repeats", &m_CollapseMessages);
			
			ImGui::PopStyleVar();
			ImGui::EndPopup();
		}
		ImGui::SameLine( ImGui::GetContentRegionAvail().x - (2.0f + (float)m_ShowFilterButtons * 5.0f) * buttonwidth);
		if (ImGui::Button("OPTIONS", buttonSize))
			ImGui::OpenPopup("OPTIONS");
		
		
		ImGui::SameLine( ImGui::GetContentRegionAvail().x - ( 1.0f + (float)m_ShowFilterButtons * 5.0f ) * buttonwidth );
		if (ImGui::Button("FILTER ...", buttonSize))
		{
			m_ShowFilterButtons ^= true;
			m_MessageFilters =  (int16_t)ConsoleMessage::Category::Trace | (int16_t)ConsoleMessage::Category::Info | (int16_t)ConsoleMessage::Category::Warn| (int16_t)ConsoleMessage::Category::Error | (int16_t)ConsoleMessage::Category::Fatal;
		}
			if (m_ShowFilterButtons)
		{
			int button_col_pop = 0;
			//------------------------------------------------------------------------
			ImGui::SameLine(ImGui::GetContentRegionAvail().x - 5.0f * (buttonwidth));
			ImGui::PushStyleColor(ImGuiCol_Text, TRACE_COL); button_col_pop++;
			if ((m_MessageFilters & (int16_t)ConsoleMessage::Category::Trace))
			{
				ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(TRACE_COL.x, TRACE_COL.y, TRACE_COL.z, 0.25f)); button_col_pop++;
			}
			if (ImGui::Button("TRACE", buttonSize))
				m_MessageFilters ^= (int16_t)ConsoleMessage::Category::Trace;
			ImGui::PopStyleColor(button_col_pop); button_col_pop = 0;

			//------------------------------------------------------------------------
			ImGui::SameLine();
			ImGui::PushStyleColor(ImGuiCol_Text, INFO_COL); button_col_pop++;
			if ((m_MessageFilters & (int16_t)ConsoleMessage::Category::Info))
			{
				ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(INFO_COL.x, INFO_COL.y, INFO_COL.z, 0.25f)); button_col_pop++;
			}
			if (ImGui::Button("INFO", buttonSize))
				m_MessageFilters ^= (int16_t)ConsoleMessage::Category::Info;
			ImGui::PopStyleColor(button_col_pop); button_col_pop = 0;

			//------------------------------------------------------------------------
			ImGui::SameLine();
			ImGui::PushStyleColor(ImGuiCol_Text, WARN_COL); button_col_pop++;
			if ((m_MessageFilters & (int16_t)ConsoleMessage::Category::Warn))
			{
				ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(WARN_COL.x, WARN_COL.y, WARN_COL.z, 0.25f)); button_col_pop++;
			}
			if (ImGui::Button("WARN", buttonSize))
				m_MessageFilters ^= (int16_t)ConsoleMessage::Category::Warn;
			ImGui::PopStyleColor(button_col_pop); button_col_pop = 0;

			//------------------------------------------------------------------------
			ImGui::SameLine();
			ImGui::PushStyleColor(ImGuiCol_Text, ERROR_COL); button_col_pop++;
			if ((m_MessageFilters & (int16_t)ConsoleMessage::Category::Error))
			{
				ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(ERROR_COL.x, ERROR_COL.y, ERROR_COL.z, 0.25f)); button_col_pop++;
			}
			if (ImGui::Button("ERROR", buttonSize))
				m_MessageFilters ^= (int16_t)ConsoleMessage::Category::Error;
			ImGui::PopStyleColor(button_col_pop); button_col_pop = 0;

			//------------------------------------------------------------------------
			ImGui::SameLine();
			ImGui::PushStyleColor(ImGuiCol_Text, FATAL_COL); button_col_pop++;
			if ((m_MessageFilters & (int16_t)ConsoleMessage::Category::Fatal))
			{
				ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(FATAL_COL.x, FATAL_COL.y, FATAL_COL.z, 0.25f)); button_col_pop++;
			}
			if (ImGui::Button("FATAL", buttonSize))
				m_MessageFilters ^= (int16_t)ConsoleMessage::Category::Fatal;
			ImGui::PopStyleColor(button_col_pop); button_col_pop = 0;
		}
		
		
		ImGui::PopStyleVar(style_pop_count);
		ImGui::PopStyleColor(color_pop_count);


		ImGui::EndChild();
	}

	// todo: add a check for the current filters before adding a selection to the m_SelectedMessage map (both level filter and text filter)
	void ConsolePanel::RenderConsole()
	{
		
		bool shift = ImGui::IsKeyDown(ImGuiKey_LeftShift) || ImGui::IsKeyDown(ImGuiKey_RightShift);
		bool ctrl = ImGui::IsKeyDown(ImGuiKey_LeftCtrl) || ImGui::IsKeyDown(ImGuiKey_RightCtrl);
		float available_y = ImGui::GetContentRegionAvail().y-3.0f;
		if (m_ShowSearchBar)
			available_y = ImGui::GetContentRegionAvail().y - ImGui::GetFrameHeightWithSpacing();

		ImGui::BeginChild("Log Messages", ImVec2(ImGui::GetContentRegionAvail().x, available_y));

		if (m_MessageBufferBegin == 0)
		{
			//m_SelectedMessage = nullptr;
			m_SelectedMessages.clear();
		}

		
		if (ImGui::IsMouseClicked(ImGuiMouseButton_Left) && !m_IsMessageInspectorHovered && !m_IsMenuHovered && !m_IsContextOpen)
		{
			
			//m_DisplayMessageInspector = false;
			//m_SelectedMessage = nullptr;
			if (!ctrl && !shift)
				m_SelectedMessages.clear();
		}
		
		m_IsContextOpen = false;
		for (uint32_t i = 0; i < m_MessageBufferBegin; i++)
		{
			int style_pop_count = 0;
			int color_pop_count = 0;
			const auto& msg = m_MessageBuffer[i];
			const auto& sel_msg = m_SelectedMessages;
			if (m_MessageFilters & (int16_t)msg.GetCategory())
			{
				if ( std::string(m_TextFilter).empty() || (msg.GetConsoleMessage().find(m_TextFilter) != std::string::npos) )
				{
					ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(5, 5)); style_pop_count++;

					switch (msg.GetCategory())
					{
					case ConsoleMessage::Category::Trace:
					{
						ImGui::PushStyleColor(ImGuiCol_Text, TRACE_COL); color_pop_count++;
						break;
					}
					case ConsoleMessage::Category::Info:
					{
						ImGui::PushStyleColor(ImGuiCol_Text, INFO_COL); color_pop_count++;
						break;
					}
					case ConsoleMessage::Category::Warn:
					{
						ImGui::PushStyleColor(ImGuiCol_Text, WARN_COL); color_pop_count++;
						break;
					}
					case ConsoleMessage::Category::Error:
					{
						ImGui::PushStyleColor(ImGuiCol_Text, ERROR_COL); color_pop_count++;
						break;
					}
					case ConsoleMessage::Category::Fatal:
					{
						ImGui::PushStyleColor(ImGuiCol_Text, FATAL_COL); color_pop_count++;
						break;
					}
					}
				
					if (m_SelectedMessages.find(i) != m_SelectedMessages.end())
					{
						ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(1.0f,1.0f,1.0f,0.1f)); color_pop_count++;
					}
					ImGui::BeginChild(i + 1, ImVec2(0, ImGui::GetFontSize() * 1.0F), false, ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoScrollbar);// | ImGuiWindowFlags_AlwaysUseWindowPadding);


					if ( ImGui::IsWindowHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Left) )
					{

						if (ctrl && !shift)
						{
							if (m_SelectedMessages.find(i) != m_SelectedMessages.end())
								m_SelectedMessages.erase(i);
							else
								m_SelectedMessages[i] = &m_MessageBuffer[i];

						}
						if (shift)
						{
							if (m_SelectedMessages.empty())
								m_SelectedMessages[i] = &m_MessageBuffer[i];
							
							uint32_t begin_idx = (m_SelectedMessages.begin())->first;
							uint32_t end_idx = std::prev(m_SelectedMessages.end())->first;

							if (i >= end_idx)
							{
								if (!ctrl)
								{
									m_SelectedMessages.clear();
									m_SelectedMessages[end_idx] = &m_MessageBuffer[end_idx];
								}
								for (uint32_t idx = end_idx; idx <= i; idx++)
									m_SelectedMessages[idx] = &m_MessageBuffer[idx];
							}
							if (i <= begin_idx)
							{
								if (!ctrl)
								{
									m_SelectedMessages.clear();
									m_SelectedMessages[begin_idx] = &m_MessageBuffer[begin_idx];
								}
								for (uint32_t idx = i; idx <= begin_idx; idx++)
								{
									m_SelectedMessages[idx] = &m_MessageBuffer[idx];
								}
							}
						}
						if (!ctrl && !shift)
						{
							m_SelectedMessages.clear();
							m_SelectedMessages[i] = &m_MessageBuffer[i];
						}
					}

					std::string messageText = msg.GetConsoleMessage();

					if (ImGui::BeginPopupContextWindow("##MESSAGECONTEXTMENU"))
					{
						m_IsContextOpen = true;
						ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1, 1, 1, 1));
						if (ImGui::MenuItem("Copy"))
						{
							std::stringstream ss;
							for (const auto& [idx, selected] : m_SelectedMessages)
							{
								ss << selected->GetConsoleMessage().c_str();
							}
							ImGui::SetClipboardText(ss.str().c_str());
						}
						ImGui::PopStyleColor();
						ImGui::EndPopup();
					}

					ImGui::SameLine();

					if (messageText.length() > 200)
					{
						size_t spacePos = messageText.find_first_of(' ', 200);
						if (spacePos != std::string::npos)
							messageText.replace(spacePos, messageText.length() - 1, "...");
					}

					ImGui::TextUnformatted(messageText.c_str());
					if (m_CollapseMessages && msg.GetCount() > 1)
					{
						ImGui::SameLine(ImGui::GetWindowWidth() - 30);
						ImGui::Text("%d", msg.GetCount());
					}
					ImGui::EndChild();
					ImGui::PopStyleColor(color_pop_count);
					ImGui::PopStyleVar(style_pop_count);
						
				}
			}
		}

		if (m_AutoScroll && m_NewMessageAdded)
		{
			ImGui::SetScrollHereY(1.0f);
			m_NewMessageAdded = false;
		}

		if (m_AutoScroll && ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
			ImGui::SetScrollHereY(1.0f);

		if (m_DisplayMessageInspector && !m_SelectedMessages.empty())
		{
			// TOOD(Peter): Ensure that this panel is always docked to the bottom of the Log panel
			ImGui::Begin("Message Inspector");

			m_IsMessageInspectorHovered = ImGui::IsWindowHovered();

			ImGui::PushTextWrapPos();
			for (const auto& [idx,msg] : m_SelectedMessages)
			{
				if (m_MessageFilters & (int16_t)msg->GetCategory())
				{
					ImGui::Text(" ID: %d ", idx);
					ImGui::SameLine();
					ImGui::Text(LevelToString(msg->GetCategory()));
					ImGui::SameLine();
					ImGui::TextUnformatted(msg->GetConsoleMessage().c_str());
				}

			}
			ImGui::PopTextWrapPos();

			ImGui::End();
		}
		else
		{
			m_IsMessageInspectorHovered = false;
		}
		ImGui::EndChild();
	}

	void ConsolePanel::PushMessage(const ConsoleMessage& message)
	{
		if (s_Instance == nullptr)
			return;

		if (message.GetCategory() == ConsoleMessage::Category::None)
			return;

		if (s_Instance->m_CollapseMessages)
		{
			for (auto& other : s_Instance->m_MessageBuffer)
			{
				if (message.GetMessageID() == other.GetMessageID())
				{
					other.m_Count++;
					return;
				}
			}
		}

		s_Instance->m_MessageBuffer[s_Instance->m_MessageBufferBegin++] = message;
		s_Instance->m_NewMessageAdded = true;

		if (s_Instance->m_MessageBufferBegin == s_MessageBufferCapacity)
			s_Instance->m_MessageBufferBegin = 0;

	}

}