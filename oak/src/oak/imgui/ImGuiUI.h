#pragma once
#include "oakpch.h"
#include "oak/renderer/Texture.h"
#include "oak/renderer/RendererAPI.h"
#include "oak/renderer/Renderer.h"
#include "oak/platform/OpenGL/OpenGLTexture.h"
#include "oak/imgui/ImGuiUtils.h"
#include "oak/imgui/ImGuiDraw.h"
#include "oak/imgui/imgui_stdlib.h"

#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "imgui.h"
#ifndef IMGUI_DEFINE_MATH_OPERATORS
#define IMGUI_DEFINE_MATH_OPERATORS
#endif
#include "imgui_internal.h"


namespace Oak::UI
{

    static int s_UIContextID = 0;
    static uint32_t s_Counter = 0;
    static char s_IDBuffer[16] = "##";

    static const char* GenerateID()
    {
        itoa(s_Counter++, s_IDBuffer + 2, 16);
        return s_IDBuffer;
    }

    static void PushID()
    {
        ImGui::PushID(s_UIContextID++);
        s_Counter = 0;
    }

    static void PopID()
    {
        ImGui::PopID();
        s_UIContextID--;
    }


    enum OakMsgBoxFlags_
    {
        OakMsgBoxFlag_OkButton = 1u << 0,
        OakMsgBoxFlag_CancelButton = 1u << 1,
        OakMsgBoxFlag_UserFunc = 1u << 2,
    };

    struct MessageBoxData
    {
        std::string Title = "";
        std::string Body = "";
        uint32_t Flags = 0;
        uint32_t Width = 0;
        uint32_t Height = 0;
        std::function<void()> UserRenderFunction;

        bool ShouldOpen = true;
    };
    static std::unordered_map<std::string, MessageBoxData> s_MessageBoxes;

    inline bool ImageButton(Ref<Texture2D> tex, ImVec2& size, int pad = 0)
    {
        
        ImTextureID user_texture_id = Utils::GetImTextureID(tex);
        ImGuiContext* g = ImGui::GetCurrentContext();
        ImGuiWindow* window = g->CurrentWindow;
        if (window->SkipItems)
            return false;

        ImGui::PushID((void*)(intptr_t)user_texture_id);
        const ImGuiID id = window->GetID("#image");
        ImGui::PopID();

        const ImVec2 padding = (pad >= 0) ? ImVec2((float)pad, (float)pad) : g->Style.FramePadding;
        //const ImVec2 padding(0, 0);

        ImGui::KeepAliveID(id);

        const ImRect bb(window->DC.CursorPos, window->DC.CursorPos + size + padding * 2);
        ImGui::ItemSize(bb);
        if (!ImGui::ItemAdd(bb, id))
            return false;

        bool hovered, held;
        bool pressed = ImGui::ButtonBehavior(bb, id, &hovered, &held);

        // Render
        const ImU32 col = ImGui::GetColorU32((held && hovered) ? ImGuiCol_ButtonActive : hovered ? ImGuiCol_ButtonHovered : ImGuiCol_Button);
        ImGui::RenderNavHighlight(bb, id);
        ImGui::RenderFrame(bb.Min, bb.Max, col, true, ImClamp((float)ImMin(padding.x, padding.y), 0.0f, g->Style.FrameRounding));

        ImVec4 tint_col = hovered ? ImVec4(1, 1, 1, 1) : ImVec4(1, 1, 1, 0.5f);

        window->DrawList->AddImage(user_texture_id, bb.Min + padding, bb.Max - padding, ImVec2(0, 0), ImVec2(1, 1), ImGui::GetColorU32(tint_col));

        return pressed;
    }



    static void ShowMessageBox(const char* title, const std::function<void()>& renderFunction, uint32_t width = 600, uint32_t height = 0)
    {
        auto& messageBoxData = s_MessageBoxes[title];
        messageBoxData.Title = fmt::format("{0}##MessageBox{1}", title, s_MessageBoxes.size() + 1);
        messageBoxData.UserRenderFunction = renderFunction;
        messageBoxData.Flags = OakMsgBoxFlag_UserFunc;
        messageBoxData.Width = width;
        messageBoxData.Height = height;
        messageBoxData.ShouldOpen = true;
    }
    static void RenderMessageBoxes()
    {
        for (auto& [key, messageBoxData] : s_MessageBoxes)
        {
            if (messageBoxData.ShouldOpen)
            {
                ImGui::OpenPopup(messageBoxData.Title.c_str());
                messageBoxData.ShouldOpen = false;
            }

            ImVec2 center = ImGui::GetMainViewport()->GetCenter();
            ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
            ImGui::SetNextWindowSize(ImVec2{ (float)messageBoxData.Width, (float)messageBoxData.Height });

            if (ImGui::BeginPopupModal(messageBoxData.Title.c_str(), nullptr, ImGuiWindowFlags_AlwaysAutoResize))
            {
                if (messageBoxData.Flags & OakMsgBoxFlag_UserFunc)
                {
                    OAK_CORE_VERIFY(messageBoxData.UserRenderFunction, "No render function provided for message box!");
                    messageBoxData.UserRenderFunction();
                }
                else
                {
                    ImGui::TextWrapped(messageBoxData.Body.c_str());

                    if (messageBoxData.Flags & OakMsgBoxFlag_OkButton)
                    {
                        if (ImGui::Button("Ok"))
                            ImGui::CloseCurrentPopup();

                        if (messageBoxData.Flags & OakMsgBoxFlag_CancelButton)
                            ImGui::SameLine();
                    }

                    if (messageBoxData.Flags & OakMsgBoxFlag_CancelButton && ImGui::Button("Cancel"))
                    {
                        ImGui::CloseCurrentPopup();
                    }
                }

                ImGui::EndPopup();
            }
        }
    }

    static void BeginDisabled(bool disabled = true)
    {
        if (disabled)
            ImGui::BeginDisabled(true);
    }

    static bool IsItemDisabled()
    {
        return ImGui::GetItemFlags() & ImGuiItemFlags_Disabled;
    }

    static void EndDisabled()
    {
        // NOTE(Peter): Cheeky hack to prevent ImGui from asserting (required due to the nature of UI::BeginDisabled)
        if (GImGui->DisabledStackSize > 0)
            ImGui::EndDisabled();
    }

	static void HelpMarker(const char* desc)
	{
		ImGui::TextDisabled("(?)");
		if (ImGui::IsItemHovered())
		{
			ImGui::BeginTooltip();
			ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
			ImGui::TextUnformatted(desc);
			ImGui::PopTextWrapPos();
			ImGui::EndTooltip();
		}
	}

    static bool PropertyGridHeader(const std::string& name, bool openByDefault = true)
    {
        ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_Framed
            | ImGuiTreeNodeFlags_SpanAvailWidth
            | ImGuiTreeNodeFlags_AllowItemOverlap
            | ImGuiTreeNodeFlags_FramePadding;


        if (openByDefault)
            treeNodeFlags |= ImGuiTreeNodeFlags_DefaultOpen;

        bool open = false;
        const float framePaddingX = 2.0f;
        const float framePaddingY = 5.0f; // affects height of the header

        Utils::ScopedStyle headerRounding(ImGuiStyleVar_FrameRounding, 0.0f);
        Utils::ScopedStyle headerPaddingAndHeight(ImGuiStyleVar_FramePadding, ImVec2{ framePaddingX, framePaddingY });

        //UI::PushID();
        ImGui::PushID(name.c_str());
        open = ImGui::TreeNodeEx("##dummy_id", treeNodeFlags, name.c_str());
       
        //UI::PopID();
        ImGui::PopID();

        return open;
    }
    static void BeginPropertyGrid(uint32_t columns = 2)
    {
        PushID();
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(2.0f, 4.0f));
        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(2.0f, 4.0f));
        ImGui::Unindent(ImGui::GetTreeNodeToLabelSpacing());
        ImGui::Columns(columns);
        ImGui::SetColumnWidth(0, ImGui::GetWindowContentRegionWidth() * 0.35);
    }

    static void EndPropertyGrid()
    {
        ImGui::Indent(ImGui::GetTreeNodeToLabelSpacing());
        ImGui::Columns(1);
        Draw::Underline(true);
        ImGui::PopStyleVar(2); // ItemSpacing, FramePadding
        Draw::ShiftCursorY(9.0f);
        PopID();
    }




    static bool Property(const char* label, std::string& value, const char* hint ="", bool disable = true)
    {
        ImGuiInputTextFlags flags = ImGuiInputTextFlags_None;
        if (disable) flags |= ImGuiInputTextFlags_ReadOnly;
        Draw::ShiftCursor(4.0f, 4.0f);
        ImGui::Text(label);
        ImGui::NextColumn();
        Draw::ShiftCursorY(2.0f);
        ImGui::PushItemWidth(-1);
        if (disable) BeginDisabled();
        bool modified = ImGui::InputTextWithHint(GenerateID(), hint, &value, flags);
        if (disable) EndDisabled();

        if (!IsItemDisabled())
            Draw::DrawItemActivityOutline(0.0f, true, Colours::Theme::accent);

        ImGui::PopItemWidth();
        ImGui::NextColumn();
        Draw::Underline();
        return modified;
    }

    static void Property(const char* label, const char* value)
    {
        Draw::ShiftCursor(4.0f, 4.0f);
        ImGui::Text(label);
        ImGui::NextColumn();
        Draw::ShiftCursorY(2.0f);
        ImGui::PushItemWidth(-1);
        BeginDisabled();
        ImGui::InputText(GenerateID(), (char*)value, 256, ImGuiInputTextFlags_ReadOnly);
        EndDisabled();

        if (!IsItemDisabled())
            Draw::DrawItemActivityOutline(0.0f, true, Colours::Theme::accent);

        ImGui::PopItemWidth();
        ImGui::NextColumn();
        Draw::Underline();
    }

    static bool Property(const char* label, char* value, size_t length)
    {
        Draw::ShiftCursor(4.0f, 4.0f);
            ImGui::Text(label);
        ImGui::NextColumn();
        Draw::ShiftCursorY(2.0f);
        ImGui::PushItemWidth(-1);

        bool modified = ImGui::InputText(GenerateID(), value, length);

        if (!IsItemDisabled())
            Draw::DrawItemActivityOutline(0.0f, true, Colours::Theme::accent);

        ImGui::PopItemWidth();
        ImGui::NextColumn();
        Draw::Underline();

        return modified;
    }

    static bool Property(const char* label, bool& value, const char* helpText = "")
    {
        bool modified = false;

        Draw::ShiftCursor(4.0f, 4.0f);
        ImGui::Text(label);
        if (std::strlen(helpText) != 0)
        {
            ImGui::SameLine();
            HelpMarker(helpText);
        }
        ImGui::NextColumn();
        Draw::ShiftCursorY(2.0f);
        ImGui::PushItemWidth(-1);

        modified = ImGui::Checkbox(GenerateID(), &value);

        if (!IsItemDisabled())
            Draw::DrawItemActivityOutline(0.0f, true, Colours::Theme::accent);

        ImGui::PopItemWidth();
        ImGui::NextColumn();
        Draw::Underline();

        return modified;
    }


    static bool Property(const char* label, int8_t& value, int8_t min = 0, int8_t max = 0)
    {
        Draw::ShiftCursor(4.0f, 4.0f);
        ImGui::Text(label);
        ImGui::NextColumn();
        Draw::ShiftCursorY(2.0f);
        ImGui::PushItemWidth(-1);

        bool modified = Draw::DragInt8(GenerateID(), &value, 1.0f, min, max);

        if (!IsItemDisabled())
            Draw::DrawItemActivityOutline(0.0f, true, Colours::Theme::accent);

        ImGui::PopItemWidth();
        ImGui::NextColumn();
        Draw::Underline();

        return modified;
    }

    static bool Property(const char* label, int16_t& value, int16_t min = 0, int16_t max = 0)
    {
        Draw::ShiftCursor(4.0f, 4.0f);
        ImGui::Text(label);
        ImGui::NextColumn();
        Draw::ShiftCursorY(2.0f);
        ImGui::PushItemWidth(-1);

        bool modified = Draw::DragInt16(GenerateID(), &value, 1.0f, min, max);

        if (!IsItemDisabled())
            Draw::DrawItemActivityOutline(0.0f, true, Colours::Theme::accent);

        ImGui::PopItemWidth();
        ImGui::NextColumn();
        Draw::Underline();

        return modified;
    }

    static bool Property(const char* label, int32_t& value, int32_t min = 0, int32_t max = 0)
    {
        Draw::ShiftCursor(4.0f, 4.0f);
        ImGui::Text(label);
        ImGui::NextColumn();
        Draw::ShiftCursorY(2.0f);
        ImGui::PushItemWidth(-1);

        bool modified = Draw::DragInt32(GenerateID(), &value, 1.0f, min, max);

        if (!IsItemDisabled())
            Draw::DrawItemActivityOutline(0.0f, true, Colours::Theme::accent);

        ImGui::PopItemWidth();
        ImGui::NextColumn();
        Draw::Underline();

        return modified;
    }

    static bool Property(const char* label, int64_t& value, int64_t min = 0, int64_t max = 0)
    {
        Draw::ShiftCursor(4.0f, 4.0f);
        ImGui::Text(label);
        ImGui::NextColumn();
        Draw::ShiftCursorY(2.0f);
        ImGui::PushItemWidth(-1);

        bool modified = Draw::DragInt64(GenerateID(), &value, 1.0f, min, max);

        if (!IsItemDisabled())
            Draw::DrawItemActivityOutline(0.0f, true, Colours::Theme::accent);

        ImGui::PopItemWidth();
        ImGui::NextColumn();
        Draw::Underline();

        return modified;
    }

    static bool Property(const char* label, uint8_t& value, uint8_t minValue = 0, uint8_t maxValue = 0)
    {
        Draw::ShiftCursor(4.0f, 4.0f);
        ImGui::Text(label);
        ImGui::NextColumn();
        Draw::ShiftCursorY(2.0f);
        ImGui::PushItemWidth(-1);

        bool modified = Draw::DragUInt8(GenerateID(), &value, 1.0f, minValue, maxValue);

        if (!IsItemDisabled())
            Draw::DrawItemActivityOutline(0.0f, true, Colours::Theme::accent);

        ImGui::PopItemWidth();
        ImGui::NextColumn();
        Draw::Underline();

        return modified;
    }

    static bool Property(const char* label, uint16_t& value, uint16_t minValue = 0, uint16_t maxValue = 0)
    {
        Draw::ShiftCursor(4.0f, 4.0f);
        ImGui::Text(label);
        ImGui::NextColumn();
        Draw::ShiftCursorY(2.0f);
        ImGui::PushItemWidth(-1);

        bool modified = Draw::DragUInt16(GenerateID(), &value, 1.0f, minValue, maxValue);

        if (!IsItemDisabled())
            Draw::DrawItemActivityOutline(0.0f, true, Colours::Theme::accent);

        ImGui::PopItemWidth();
        ImGui::NextColumn();
        Draw::Underline();

        return modified;
    }

    static bool Property(const char* label, uint32_t& value, uint32_t minValue = 0, uint32_t maxValue = 0)
    {
        Draw::ShiftCursor(4.0f, 4.0f);
        ImGui::Text(label);
        ImGui::NextColumn();
        Draw::ShiftCursorY(2.0f);
        ImGui::PushItemWidth(-1);

        bool modified = Draw::DragUInt32(GenerateID(), &value, 1.0f, minValue, maxValue);

        if (!IsItemDisabled())
            Draw::DrawItemActivityOutline(0.0f, true, Colours::Theme::accent);

        ImGui::PopItemWidth();
        ImGui::NextColumn();
        Draw::Underline();

        return modified;
    }

    static bool Property(const char* label, uint64_t& value, uint64_t minValue = 0, uint64_t maxValue = 0)
    {
        Draw::ShiftCursor(4.0f, 4.0f);
        ImGui::Text(label);
        ImGui::NextColumn();
        Draw::ShiftCursorY(2.0f);
        ImGui::PushItemWidth(-1);

        bool modified = Draw::DragUInt64(GenerateID(), &value, 1.0f, minValue, maxValue);

        if (!IsItemDisabled())
            Draw::DrawItemActivityOutline(0.0f, true, Colours::Theme::accent);

        ImGui::PopItemWidth();
        ImGui::NextColumn();
        Draw::Underline();

        return modified;
    }

    static bool Property(const char* label, float& value, float delta = 0.1f, float min = 0.0f, float max = 0.0f, const char* helpText = "")
    {
        Draw::ShiftCursor(4.0f, 4.0f);
        ImGui::Text(label);
        if (std::strlen(helpText) != 0)
        {
            ImGui::SameLine();
            HelpMarker(helpText);
        }
        ImGui::NextColumn();
        Draw::ShiftCursorY(2.0f);
        ImGui::PushItemWidth(-1);

        bool modified = Draw::DragFloat(GenerateID(), &value, delta, min, max);

        if (!IsItemDisabled())
            Draw::DrawItemActivityOutline(0.0f, true, Colours::Theme::accent);

        ImGui::PopItemWidth();
        ImGui::NextColumn();
        Draw::Underline();

        return modified;
    }

    static bool Property(const char* label, double& value, float delta = 0.1f, double min = 0.0, double max = 0.0)
    {
        Draw::ShiftCursor(4.0f, 4.0f);
        ImGui::Text(label);
        ImGui::NextColumn();
        Draw::ShiftCursorY(2.0f);
        ImGui::PushItemWidth(-1);

        bool modified = Draw::DragDouble(GenerateID(), &value, delta, min, max);

        if (!IsItemDisabled())
            Draw::DrawItemActivityOutline(0.0f, true, Colours::Theme::accent);

        ImGui::PopItemWidth();
        ImGui::NextColumn();
        Draw::Underline();

        return modified;
    }

    static bool Property(const char* label, glm::uvec2& value, float delta = 1.0f, float min = 0.0f, float max = 0.0f)
    {
        Draw::ShiftCursor(4.0f, 4.0f);
        ImGui::Text(label);
        ImGui::NextColumn();
        Draw::ShiftCursorY(2.0f);
        ImGui::PushItemWidth(-1);

        bool modified = ImGui::DragScalarN(GenerateID(), ImGuiDataType_U32, glm::value_ptr(value), 2, delta, &min, &max, "%0001d", ImGuiSliderFlags_None);


        if (!IsItemDisabled())
            Draw::DrawItemActivityOutline(0.0f, true, Colours::Theme::accent);

        ImGui::PopItemWidth();
        ImGui::NextColumn();
        Draw::Underline();

        return modified;
    }


    static bool Property(const char* label, glm::ivec2& value, float delta = 1.0f, float min = 0.0f, float max = 0.0f)
    {
        Draw::ShiftCursor(4.0f, 4.0f);
        ImGui::Text(label);
        ImGui::NextColumn();
        Draw::ShiftCursorY(2.0f);
        ImGui::PushItemWidth(-1);

        bool modified = ImGui::DragInt2(GenerateID(), glm::value_ptr(value), delta, min, max);


        if (!IsItemDisabled())
            Draw::DrawItemActivityOutline(0.0f, true, Colours::Theme::accent);

        ImGui::PopItemWidth();
        ImGui::NextColumn();
        Draw::Underline();

        return modified;
    }

    static bool Property(const char* label, glm::vec2& value, float delta = 0.1f, float min = 0.0f, float max = 0.0f)
    {
        Draw::ShiftCursor(4.0f, 4.0f);
        ImGui::Text(label);
        ImGui::NextColumn();
        Draw::ShiftCursorY(2.0f);
        ImGui::PushItemWidth(-1);

        bool modified = ImGui::DragFloat2(GenerateID(), glm::value_ptr(value), delta, min, max);


        if (!IsItemDisabled())
            Draw::DrawItemActivityOutline(0.0f, true, Colours::Theme::accent);

        ImGui::PopItemWidth();
        ImGui::NextColumn();
        Draw::Underline();

        return modified;
    }

    static bool Property(const char* label, glm::vec3& value, float delta = 0.1f, float min = 0.0f, float max = 0.0f, const char* helpText = "")
    {
        Draw::ShiftCursor(4.0f, 4.0f);
        ImGui::Text(label);

        if (std::strlen(helpText) != 0)
        {
            ImGui::SameLine();
            HelpMarker(helpText);
        }

        ImGui::NextColumn();
        Draw::ShiftCursorY(2.0f);
        ImGui::PushItemWidth(-1);

        bool modified = ImGui::DragFloat3(GenerateID(), glm::value_ptr(value), delta, min, max);

        if (!IsItemDisabled())
            Draw::DrawItemActivityOutline(0.0f, true, Colours::Theme::accent);

        ImGui::PopItemWidth();
        ImGui::NextColumn();
        Draw::Underline();

        return modified;
    }

    static bool PropertyDropdown(const char* label, const char** options, int32_t optionCount, int32_t* selected)
    {
        const char* current = options[*selected];
        Draw::ShiftCursor(4.0f, 4.0f);
        ImGui::Text(label);
        ImGui::NextColumn();
        Draw::ShiftCursorY(2.0f);
        ImGui::PushItemWidth(-1);

        bool modified = false;

        const std::string id = "##" + std::string(label);
        if (ImGui::BeginCombo(id.c_str(), current))
        {
            for (int i = 0; i < optionCount; i++)
            {
                const bool is_selected = (current == options[i]);
                if (ImGui::Selectable(options[i], is_selected))
                {
                    current = options[i];
                    *selected = i;
                    modified = true;
                }
                if (is_selected)
                    ImGui::SetItemDefaultFocus();
            }
            ImGui::EndCombo();
        }

        if (!IsItemDisabled())
            Draw::DrawItemActivityOutline(0.0f, true, Colours::Theme::accent);

        ImGui::PopItemWidth();
        ImGui::NextColumn();
        Draw::Underline();

        return modified;
    }

    static bool PropertyDropdown(const char* label, const std::vector<std::string>& options, int32_t optionCount, int32_t* selected)
    {
        const char* current = options[*selected].c_str();

        Draw::ShiftCursor(4.0f, 4.0f);
        ImGui::Text(label);
        ImGui::NextColumn();
        Draw::ShiftCursorY(2.0f);
        ImGui::PushItemWidth(-1);

        bool modified = false;

        const std::string id = "##" + std::string(label);
        if (ImGui::BeginCombo(id.c_str(), current))
        {
            for (int i = 0; i < optionCount; i++)
            {
                const bool is_selected = (current == options[i]);
                if (ImGui::Selectable(options[i].c_str(), is_selected))
                {
                    current = options[i].c_str();
                    *selected = i;
                    modified = true;
                }
                if (is_selected)
                    ImGui::SetItemDefaultFocus();
            }
            ImGui::EndCombo();
        }

        if (!IsItemDisabled())
            Draw::DrawItemActivityOutline(0.0f, true, Colours::Theme::accent);

        ImGui::PopItemWidth();
        ImGui::NextColumn();
        Draw::Underline();

        return modified;
    }

    static bool PropertyColor(const char* label, glm::vec3& value)
    {
        Draw::ShiftCursor(4.0f, 4.0f);
        ImGui::Text(label);
        ImGui::NextColumn();
        Draw::ShiftCursorY(2.0f);
        ImGui::PushItemWidth(-1);

        bool modified = ImGui::ColorEdit3(GenerateID(), glm::value_ptr(value));

        if (!IsItemDisabled())
            Draw::DrawItemActivityOutline(2.0f, true, Colours::Theme::accent);

        ImGui::PopItemWidth();
        ImGui::NextColumn();
        Draw::Underline();

        return modified;
    }

    static bool PropertyColor(const char* label, ImVec4& value)
    {
        Draw::ShiftCursor(4.0f, 4.0f);
        ImGui::Text(label);
        ImGui::NextColumn();
        Draw::ShiftCursorY(2.0f);
        ImGui::PushItemWidth(-1);

        bool modified = ImGui::ColorEdit4(GenerateID(), &value.x);

        if (!IsItemDisabled())
            Draw::DrawItemActivityOutline(2.0f, true, Colours::Theme::accent);

        ImGui::PopItemWidth();
        ImGui::NextColumn();
        Draw::Underline();

        return modified;
    }
    
    static int s_CheckboxCount = 0;

    static void BeginCheckboxGroup(const char* label)
    {
        Draw::ShiftCursor(4.0f, 4.0f);
        ImGui::Text(label);;
        ImGui::NextColumn();
        ImGui::PushItemWidth(-1);
    }

    static bool PropertyCheckboxGroup(const char* label, bool& value, int cols=2)
    {
        bool modified = false;

        if (s_CheckboxCount++ % cols != 0)
            ImGui::SameLine();

        if (ImGui::Checkbox(GenerateID(), &value))
            modified = true;

        if (!IsItemDisabled())
            Draw::DrawItemActivityOutline(0.0f, true, Colours::Theme::accent);

        ImGui::SameLine();
        ImGui::Text(label); 
        return modified;
    }
    
    static bool PropertyCheckboxColorGroup(const char* label, bool& value, ImVec4 col, int cols=2)
    {
        bool modified = false;

        if (s_CheckboxCount++ % cols != 0)
            ImGui::SameLine();

        ImGui::PushStyleColor(ImGuiCol_CheckMark, col);
        col.w = 0.5;
        ImGui::PushStyleColor(ImGuiCol_FrameBg, col);
        if (ImGui::Checkbox(GenerateID(), &value))
            modified = true;
        ImGui::PopStyleColor(2);
        
        if (!IsItemDisabled())
            Draw::DrawItemActivityOutline(0.0f, true, Colours::Theme::accent);


        ImGui::SameLine();
        ImGui::Text(label);

        return modified;
    }

    static void EndCheckboxGroup()
    {
        ImGui::PopItemWidth();
        ImGui::NextColumn();
        s_CheckboxCount = 0;
    }




}