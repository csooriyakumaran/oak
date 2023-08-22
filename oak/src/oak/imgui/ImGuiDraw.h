
#pragma once
#include "oakpch.h"
#include "oak/imgui/ImGuiColours.h"
#include "imgui.h"
#ifndef IMGUI_DEFINE_MATH_OPERATORS
#define IMGUI_DEFINE_MATH_OPERATORS
#endif
#include "imgui_internal.h"


namespace Oak::UI::Draw
{

    //=========================================================================================
    /// Lines
    static void Underline(bool fullWidth = false, float offsetX = 0.0f, float offsetY = -1.0f)
    {
        if (fullWidth)
        {
            if (ImGui::GetCurrentWindow()->DC.CurrentColumns != nullptr)
                ImGui::PushColumnsBackground();
            else if (ImGui::GetCurrentTable() != nullptr)
                ImGui::TablePushBackgroundChannel();
        }

        const float width = fullWidth ? ImGui::GetWindowWidth() : ImGui::GetContentRegionAvail().x;
        const ImVec2 cursor = ImGui::GetCursorScreenPos();
        ImGui::GetWindowDrawList()->AddLine(ImVec2(cursor.x + offsetX, cursor.y + offsetY),
            ImVec2(cursor.x + width, cursor.y + offsetY),
            Colours::Theme::backgroundDark, 1.0f);

        if (fullWidth)
        {
            if (ImGui::GetCurrentWindow()->DC.CurrentColumns != nullptr)
                ImGui::PopColumnsBackground();
            else if (ImGui::GetCurrentTable() != nullptr)
                ImGui::TablePopBackgroundChannel();
        }
    }

    //=========================================================================================
    /// Rectangle

    static inline ImRect GetItemRect()
    {
        return ImRect(ImGui::GetItemRectMin(), ImGui::GetItemRectMax());
    }

    static inline ImRect RectExpanded(const ImRect& rect, float x, float y)
    {
        ImRect result = rect;
        result.Min.x -= x;
        result.Min.y -= y;
        result.Max.x += x;
        result.Max.y += y;
        return result;
    }

    static inline ImRect RectOffset(const ImRect& rect, float x, float y)
    {
        ImRect result = rect;
        result.Min.x += x;
        result.Min.y += y;
        result.Max.x += x;
        result.Max.y += y;
        return result;
    }

    static inline ImRect RectOffset(const ImRect& rect, ImVec2 xy)
    {
        return RectOffset(rect, xy.x, xy.y);
    }


    /// Border =========================================================
    static void DrawItemActivityOutline(float rounding = 0.0f, bool drawWhenInactive = false, ImColor colourWhenActive = ImColor(80, 80, 80))
    {
        auto* drawList = ImGui::GetWindowDrawList();
        const ImRect rect = RectExpanded(GetItemRect(), 1.0f, 1.0f);
        if (ImGui::IsItemHovered() && !ImGui::IsItemActive())
        {
            drawList->AddRect(rect.Min, rect.Max,
                 ImColor(60, 60, 60), rounding, 0, 1.5f);
        }
        if (ImGui::IsItemActive())
        {
            drawList->AddRect(rect.Min, rect.Max,
                 colourWhenActive, rounding, 0, 1.0f);
        }
        else if (!ImGui::IsItemHovered() && drawWhenInactive)
        {
            drawList->AddRect(rect.Min, rect.Max,
                 ImColor(50, 50, 50), rounding, 0, 1.0f);
        }
    };


    /// Cursor =========================================================

    static void ShiftCursorX(float distance)
    {
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + distance);
    }

    static void ShiftCursorY(float distance)
    {
        ImGui::SetCursorPosY(ImGui::GetCursorPosY() + distance);
    }

    static void ShiftCursor(float x, float y)
    {
        const ImVec2 cursor = ImGui::GetCursorPos();
        ImGui::SetCursorPos(ImVec2(cursor.x + x, cursor.y + y));
    }

	//=========================================================================================
	/// Custom Drag*** / Input*** / Slider***

	static int FormatString(char* buf, size_t buf_size, const char* fmt, ...)
	{
		va_list args;
		va_start(args, fmt);
#ifdef IMGUI_USE_STB_SPRINTF
		int w = stbsp_vsnprintf(buf, (int)buf_size, fmt, args);
#else
		int w = vsnprintf(buf, buf_size, fmt, args);
#endif
		va_end(args);
		if (buf == NULL)
			return w;
		if (w == -1 || w >= (int)buf_size)
			w = (int)buf_size - 1;
		buf[w] = 0;
		return w;
	}

	static bool DragScalar(const char* label, ImGuiDataType data_type, void* p_data, float v_speed, const void* p_min, const void* p_max, const char* format, ImGuiSliderFlags flags)
	{
		ImGuiWindow* window = ImGui::GetCurrentWindow();
		if (window->SkipItems)
			return false;

		ImGuiContext& g = *GImGui;
		const ImGuiStyle& style = g.Style;
		const ImGuiID id = window->GetID(label);
		const float w = ImGui::CalcItemWidth();

		const ImVec2 label_size = ImGui::CalcTextSize(label, NULL, true);
		const ImRect frame_bb(window->DC.CursorPos, ImVec2(window->DC.CursorPos.x + w, window->DC.CursorPos.y + (label_size.y + style.FramePadding.y * 2.0f)));
		const ImRect total_bb(frame_bb.Min, ImVec2(frame_bb.Max.x + (label_size.x > 0.0f ? style.ItemInnerSpacing.x + label_size.x : 0.0f), frame_bb.Max.y));

		const bool temp_input_allowed = (flags & ImGuiSliderFlags_NoInput) == 0;
		ImGui::ItemSize(total_bb, style.FramePadding.y);
		if (!ImGui::ItemAdd(total_bb, id, &frame_bb, temp_input_allowed ? ImGuiItemFlags_Inputable : 0))
			return false;

		// Default format string when passing NULL
		if (format == NULL)
			format = ImGui::DataTypeGetInfo(data_type)->PrintFmt;

		// Tabbing or CTRL-clicking on Drag turns it into an InputText
		const bool hovered = ImGui::ItemHoverable(frame_bb, id);
		bool temp_input_is_active = temp_input_allowed && ImGui::TempInputIsActive(id);
		if (!temp_input_is_active)
		{
			const bool input_requested_by_tabbing = temp_input_allowed && (g.LastItemData.StatusFlags & ImGuiItemStatusFlags_FocusedByTabbing) != 0;
			const bool clicked = (hovered && g.IO.MouseClicked[0]);
			const bool double_clicked = (hovered && g.IO.MouseClickedCount[0] == 2);
			if (input_requested_by_tabbing || clicked || double_clicked || g.NavActivateId == id || g.NavActivateInputId == id)
			{
				ImGui::SetActiveID(id, window);
				ImGui::SetFocusID(id, window);
				ImGui::FocusWindow(window);
				g.ActiveIdUsingNavDirMask = (1 << ImGuiDir_Left) | (1 << ImGuiDir_Right);
				if (temp_input_allowed)
					if (input_requested_by_tabbing || (clicked && g.IO.KeyCtrl) || double_clicked || g.NavActivateInputId == id)
						temp_input_is_active = true;
			}

			// Experimental: simple click (without moving) turns Drag into an InputText
			if (g.IO.ConfigDragClickToInputText && temp_input_allowed && !temp_input_is_active)
				if (g.ActiveId == id && hovered && g.IO.MouseReleased[0] && !ImGui::IsMouseDragPastThreshold(0, g.IO.MouseDragThreshold * 0.5f))
				{
					g.NavActivateId = g.NavActivateInputId = id;
					g.NavActivateFlags = ImGuiActivateFlags_PreferInput;
					temp_input_is_active = true;
				}
		}

		if (temp_input_is_active)
		{
			// Only clamp CTRL+Click input when ImGuiSliderFlags_AlwaysClamp is set
			const bool is_clamp_input = (flags & ImGuiSliderFlags_AlwaysClamp) != 0 && (p_min == NULL || p_max == NULL || ImGui::DataTypeCompare(data_type, p_min, p_max) < 0);
			return ImGui::TempInputScalar(frame_bb, id, label, data_type, p_data, format, is_clamp_input ? p_min : NULL, is_clamp_input ? p_max : NULL);
		}

		// Draw frame
		const ImU32 frame_col = ImGui::GetColorU32(g.ActiveId == id ? ImGuiCol_FrameBgActive : hovered ? ImGuiCol_FrameBgHovered : ImGuiCol_FrameBg);
		ImGui::RenderNavHighlight(frame_bb, id);
		ImGui::RenderFrame(frame_bb.Min, frame_bb.Max, frame_col, true, style.FrameRounding);

		// Drag behavior
		const bool value_changed = ImGui::DragBehavior(id, data_type, p_data, v_speed, p_min, p_max, format, flags);
		if (value_changed)
			ImGui::MarkItemEdited(id);
		
		const bool mixed_value = (g.CurrentItemFlags & ImGuiItemFlags_MixedValue) != 0;

		// Display value using user-provided display format so user can add prefix/suffix/decorations to the value.
		char value_buf[64];
		const char* value_buf_end = value_buf + (mixed_value ? FormatString(value_buf, IM_ARRAYSIZE(value_buf), "---") : ImGui::DataTypeFormatString(value_buf, IM_ARRAYSIZE(value_buf), data_type, p_data, format));
		if (g.LogEnabled)
			ImGui::LogSetNextTextDecoration("{", "}");
		ImGui::RenderTextClipped(frame_bb.Min, frame_bb.Max, value_buf, value_buf_end, NULL, ImVec2(0.5f, 0.5f));

		if (label_size.x > 0.0f)
			ImGui::RenderText(ImVec2(frame_bb.Max.x + style.ItemInnerSpacing.x, frame_bb.Min.y + style.FramePadding.y), label);

		IMGUI_TEST_ENGINE_ITEM_INFO(id, label, g.LastItemData.StatusFlags);
		return value_changed;
	}

	static bool DragFloat(const char* label, float* v, float v_speed = 1.0f, float v_min = 0.0f, float v_max = 0.0f, const char* format = "%.3f", ImGuiSliderFlags flags = 0)
	{
		return DragScalar(label, ImGuiDataType_Float, v, v_speed, &v_min, &v_max, format, flags);
	}

	static bool DragDouble(const char* label, double* v, float v_speed = 1.0f, double v_min = 0.0, double v_max = 0.0, const char* format = "%.3f", ImGuiSliderFlags flags = 0)
	{
		return DragScalar(label, ImGuiDataType_Double, v, v_speed, &v_min, &v_max, format, flags);
	}

	static bool DragInt8(const char* label, int8_t* v, float v_speed = 1.0f, int8_t v_min = 0, int8_t v_max = 0, const char* format = "%d", ImGuiSliderFlags flags = 0)
	{
		return DragScalar(label, ImGuiDataType_S8, v, v_speed, &v_min, &v_max, format, flags);
	}

	static bool DragInt16(const char* label, int16_t* v, float v_speed = 1.0f, int16_t v_min = 0, int16_t v_max = 0, const char* format = "%d", ImGuiSliderFlags flags = 0)
	{
		return DragScalar(label, ImGuiDataType_S16, v, v_speed, &v_min, &v_max, format, flags);
	}

	static bool DragInt32(const char* label, int32_t* v, float v_speed = 1.0f, int32_t v_min = 0, int32_t v_max = 0, const char* format = "%d", ImGuiSliderFlags flags = 0)
	{
		return DragScalar(label, ImGuiDataType_S32, v, v_speed, &v_min, &v_max, format, flags);
	}

	static bool DragInt64(const char* label, int64_t* v, float v_speed = 1.0f, int64_t v_min = 0, int64_t v_max = 0, const char* format = "%d", ImGuiSliderFlags flags = 0)
	{
		return DragScalar(label, ImGuiDataType_S64, v, v_speed, &v_min, &v_max, format, flags);
	}

	static bool DragUInt8(const char* label, uint8_t* v, float v_speed = 1.0f, uint8_t v_min = 0, uint8_t v_max = 0, const char* format = "%d", ImGuiSliderFlags flags = 0)
	{
		return DragScalar(label, ImGuiDataType_U8, v, v_speed, &v_min, &v_max, format, flags);
	}

	static bool DragUInt16(const char* label, uint16_t* v, float v_speed = 1.0f, uint16_t v_min = 0, uint16_t v_max = 0, const char* format = "%d", ImGuiSliderFlags flags = 0)
	{
		return DragScalar(label, ImGuiDataType_U16, v, v_speed, &v_min, &v_max, format, flags);
	}

	static bool DragUInt32(const char* label, uint32_t* v, float v_speed = 1.0f, uint32_t v_min = 0, uint32_t v_max = 0, const char* format = "%d", ImGuiSliderFlags flags = 0)
	{
		return DragScalar(label, ImGuiDataType_U32, v, v_speed, &v_min, &v_max, format, flags);
	}

	static bool DragUInt64(const char* label, uint64_t* v, float v_speed = 1.0f, uint64_t v_min = 0, uint64_t v_max = 0, const char* format = "%d", ImGuiSliderFlags flags = 0)
	{
		return DragScalar(label, ImGuiDataType_U64, v, v_speed, &v_min, &v_max, format, flags);
	}

	static bool InputFloat(const char* label, float* v, float step = 0.0f, float step_fast = 0.0f, const char* format = "%.3f", ImGuiInputTextFlags flags = 0)
	{
		return ImGui::InputScalar(label, ImGuiDataType_Float, v, &step, &step_fast, format, flags);
	}

	static bool InputDouble(const char* label, double* v, double step = 0.0, double step_fast = 0.0, const char* format = "%.6f", ImGuiInputTextFlags flags = 0)
	{
		return ImGui::InputScalar(label, ImGuiDataType_Double, v, &step, &step_fast, format, flags);
	}

	static bool InputInt8(const char* label, int8_t* v, int8_t step = 1, int8_t step_fast = 1, ImGuiInputTextFlags flags = 0)
	{
		return ImGui::InputScalar(label, ImGuiDataType_S8, v, &step, &step_fast, "%d", flags);
	}

	static bool InputInt16(const char* label, int16_t* v, int16_t step = 1, int16_t step_fast = 10, ImGuiInputTextFlags flags = 0)
	{
		return ImGui::InputScalar(label, ImGuiDataType_S16, v, &step, &step_fast, "%d", flags);
	}

	static bool InputInt32(const char* label, int32_t* v, int32_t step = 1, int32_t step_fast = 100, ImGuiInputTextFlags flags = 0)
	{
		return ImGui::InputScalar(label, ImGuiDataType_S32, v, &step, &step_fast, "%d", flags);
	}

	static bool InputInt64(const char* label, int64_t* v, int64_t step = 1, int64_t step_fast = 1000, ImGuiInputTextFlags flags = 0)
	{
		return ImGui::InputScalar(label, ImGuiDataType_S64, v, &step, &step_fast, "%d", flags);
	}

	static bool InputUInt8(const char* label, uint8_t* v, uint8_t step = 1, uint8_t step_fast = 1, ImGuiInputTextFlags flags = 0)
	{
		return ImGui::InputScalar(label, ImGuiDataType_U8, v, &step, &step_fast, "%d", flags);
	}

	static bool InputUInt16(const char* label, uint16_t* v, uint16_t step = 1, uint16_t step_fast = 10, ImGuiInputTextFlags flags = 0)
	{
		return ImGui::InputScalar(label, ImGuiDataType_U16, v, &step, &step_fast, "%d", flags);
	}

	static bool InputUInt32(const char* label, uint32_t* v, uint32_t step = 1, uint32_t step_fast = 100, ImGuiInputTextFlags flags = 0)
	{
		return ImGui::InputScalar(label, ImGuiDataType_U32, v, &step, &step_fast, "%d", flags);
	}

	static bool InputUInt64(const char* label, uint64_t* v, uint64_t step = 1, uint64_t step_fast = 1000, ImGuiInputTextFlags flags = 0)
	{
		return ImGui::InputScalar(label, ImGuiDataType_U64, v, &step, &step_fast, "%d", flags);
	}


}