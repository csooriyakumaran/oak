#include "oakpch.h"
#include "ImGuiStyleSerializer.h"

#include "oak/utilities/YAMLSerialization.h"

namespace Oak
{

	ImGuiStyleSerializer::ImGuiStyleSerializer(ImGuiStyle& style)
		: m_Style(style)
	{
	}

	void ImGuiStyleSerializer::Serialize(const std::string& filepath)
	{
		YAML::Emitter out;
		out << YAML::BeginMap;
		out << YAML::Key << "THEME" << YAML::Value << "User Defined";
		out << YAML::Key << "STYLE" << YAML::Value << YAML::BeginMap;
		
		out << YAML::Key << "Alpha"							<< YAML::Value << m_Style.Alpha;
		out << YAML::Key << "DisabledAlpha"					<< YAML::Value << m_Style.DisabledAlpha;
		out << YAML::Key << "WindowPadding"					<< YAML::Value << m_Style.WindowPadding;
		out << YAML::Key << "WindowRounding"				<< YAML::Value << m_Style.WindowRounding;
		out << YAML::Key << "WindowBorderSize"				<< YAML::Value << m_Style.WindowBorderSize;
		out << YAML::Key << "WindowMinSize"					<< YAML::Value << m_Style.WindowMinSize;
		out << YAML::Key << "WindowTitleAlign"				<< YAML::Value << m_Style.WindowTitleAlign;
		out << YAML::Key << "WindowMenuButtonPosition"		<< YAML::Value << m_Style.WindowMenuButtonPosition;
		out << YAML::Key << "ChildRounding"					<< YAML::Value << m_Style.ChildRounding;
		out << YAML::Key << "ChildBorderSize"				<< YAML::Value << m_Style.ChildBorderSize;
		out << YAML::Key << "PopupRounding"					<< YAML::Value << m_Style.PopupRounding;
		out << YAML::Key << "PopupBorderSize"				<< YAML::Value << m_Style.PopupBorderSize;
		out << YAML::Key << "FramePadding"					<< YAML::Value << m_Style.FramePadding;
		out << YAML::Key << "FrameRounding"					<< YAML::Value << m_Style.FrameRounding;
		out << YAML::Key << "FrameBorderSize"				<< YAML::Value << m_Style.FrameBorderSize;
		out << YAML::Key << "ItemSpacing"					<< YAML::Value << m_Style.ItemSpacing;
		out << YAML::Key << "ItemInnerSpacing"				<< YAML::Value << m_Style.ItemInnerSpacing;
		out << YAML::Key << "CellPadding"					<< YAML::Value << m_Style.CellPadding;
		out << YAML::Key << "TouchExtraPadding"				<< YAML::Value << m_Style.TouchExtraPadding;
		out << YAML::Key << "IndentSpacing"					<< YAML::Value << m_Style.IndentSpacing;
		out << YAML::Key << "ColumnsMinSpacing"				<< YAML::Value << m_Style.ColumnsMinSpacing;
		out << YAML::Key << "ScrollbarSize"					<< YAML::Value << m_Style.ScrollbarSize;
		out << YAML::Key << "ScrollbarRounding"				<< YAML::Value << m_Style.ScrollbarRounding;
		out << YAML::Key << "GrabMinSize"					<< YAML::Value << m_Style.GrabMinSize;
		out << YAML::Key << "GrabRounding"					<< YAML::Value << m_Style.GrabRounding;
		out << YAML::Key << "LayoutAlign"					<< YAML::Value << m_Style.LayoutAlign;
		out << YAML::Key << "LogSliderDeadzone"				<< YAML::Value << m_Style.LogSliderDeadzone;
		out << YAML::Key << "TabRounding"					<< YAML::Value << m_Style.TabRounding;
		out << YAML::Key << "TabBorderSize"					<< YAML::Value << m_Style.TabBorderSize;
		out << YAML::Key << "TabMinWidthForCloseButton"		<< YAML::Value << m_Style.TabMinWidthForCloseButton;
		out << YAML::Key << "ColorButtonPosition"			<< YAML::Value << m_Style.ColorButtonPosition;
		out << YAML::Key << "ButtonTextAlign"				<< YAML::Value << m_Style.ButtonTextAlign;
		out << YAML::Key << "SelectableTextAlign"			<< YAML::Value << m_Style.SelectableTextAlign;
		out << YAML::Key << "DisplayWindowPadding"			<< YAML::Value << m_Style.DisplayWindowPadding;
		out << YAML::Key << "DisplaySafeAreaPadding"		<< YAML::Value << m_Style.DisplaySafeAreaPadding;
		out << YAML::Key << "MouseCursorScale"				<< YAML::Value << m_Style.MouseCursorScale;
		out << YAML::Key << "AntiAliasedLines"				<< YAML::Value << m_Style.AntiAliasedLines;
		out << YAML::Key << "AntiAliasedLinesUseTex"		<< YAML::Value << m_Style.AntiAliasedLinesUseTex;
		out << YAML::Key << "AntiAliasedFill"				<< YAML::Value << m_Style.AntiAliasedFill;
		out << YAML::Key << "CurveTessellationTol"			<< YAML::Value << m_Style.CurveTessellationTol;
		out << YAML::Key << "CircleTessellationMaxError"	<< YAML::Value << m_Style.CircleTessellationMaxError;

		out << YAML::EndMap; // STYLE

		out << YAML::Key << "COLORS" << YAML::Value << YAML::BeginMap;

		out << YAML::Key << "ImGuiCol_Text"					<< YAML::Value << m_Style.Colors[ImGuiCol_Text];
		out << YAML::Key << "ImGuiCol_TextDisabled"			<< YAML::Value << m_Style.Colors[ImGuiCol_TextDisabled];
		out << YAML::Key << "ImGuiCol_WindowBg"				<< YAML::Value << m_Style.Colors[ImGuiCol_WindowBg];
		out << YAML::Key << "ImGuiCol_ChildBg"				<< YAML::Value << m_Style.Colors[ImGuiCol_ChildBg];
		out << YAML::Key << "ImGuiCol_PopupBg"				<< YAML::Value << m_Style.Colors[ImGuiCol_PopupBg];
		out << YAML::Key << "ImGuiCol_Border"				<< YAML::Value << m_Style.Colors[ImGuiCol_Border];
		out << YAML::Key << "ImGuiCol_BorderShadow"			<< YAML::Value << m_Style.Colors[ImGuiCol_BorderShadow];
		out << YAML::Key << "ImGuiCol_FrameBg"				<< YAML::Value << m_Style.Colors[ImGuiCol_FrameBg];
		out << YAML::Key << "ImGuiCol_FrameBgHovered"		<< YAML::Value << m_Style.Colors[ImGuiCol_FrameBgHovered];
		out << YAML::Key << "ImGuiCol_FrameBgActive"		<< YAML::Value << m_Style.Colors[ImGuiCol_FrameBgActive];
		out << YAML::Key << "ImGuiCol_TitleBg"				<< YAML::Value << m_Style.Colors[ImGuiCol_TitleBg];
		out << YAML::Key << "ImGuiCol_TitleBgActive"		<< YAML::Value << m_Style.Colors[ImGuiCol_TitleBgActive];
		out << YAML::Key << "ImGuiCol_TitleBgCollapsed"		<< YAML::Value << m_Style.Colors[ImGuiCol_TitleBgCollapsed];
		out << YAML::Key << "ImGuiCol_MenuBarBg"			<< YAML::Value << m_Style.Colors[ImGuiCol_MenuBarBg];
		out << YAML::Key << "ImGuiCol_ScrollbarBg"			<< YAML::Value << m_Style.Colors[ImGuiCol_ScrollbarBg];
		out << YAML::Key << "ImGuiCol_ScrollbarGrab"		<< YAML::Value << m_Style.Colors[ImGuiCol_ScrollbarGrab];
		out << YAML::Key << "ImGuiCol_ScrollbarGrabHovered"	<< YAML::Value << m_Style.Colors[ImGuiCol_ScrollbarGrabHovered];
		out << YAML::Key << "ImGuiCol_ScrollbarGrabActive"	<< YAML::Value << m_Style.Colors[ImGuiCol_ScrollbarGrabActive];
		out << YAML::Key << "ImGuiCol_CheckMark"			<< YAML::Value << m_Style.Colors[ImGuiCol_CheckMark];
		out << YAML::Key << "ImGuiCol_SliderGrab"			<< YAML::Value << m_Style.Colors[ImGuiCol_SliderGrab];
		out << YAML::Key << "ImGuiCol_SliderGrabActive"		<< YAML::Value << m_Style.Colors[ImGuiCol_SliderGrabActive];
		out << YAML::Key << "ImGuiCol_Button"				<< YAML::Value << m_Style.Colors[ImGuiCol_Button];
		out << YAML::Key << "ImGuiCol_ButtonHovered"		<< YAML::Value << m_Style.Colors[ImGuiCol_ButtonHovered];
		out << YAML::Key << "ImGuiCol_ButtonActive"			<< YAML::Value << m_Style.Colors[ImGuiCol_ButtonActive];
		out << YAML::Key << "ImGuiCol_Header"				<< YAML::Value << m_Style.Colors[ImGuiCol_Header];
		out << YAML::Key << "ImGuiCol_HeaderHovered"		<< YAML::Value << m_Style.Colors[ImGuiCol_HeaderHovered];
		out << YAML::Key << "ImGuiCol_HeaderActive"			<< YAML::Value << m_Style.Colors[ImGuiCol_HeaderActive];
		out << YAML::Key << "ImGuiCol_Separator"			<< YAML::Value << m_Style.Colors[ImGuiCol_Separator];
		out << YAML::Key << "ImGuiCol_SeparatorHovered"		<< YAML::Value << m_Style.Colors[ImGuiCol_SeparatorHovered];
		out << YAML::Key << "ImGuiCol_SeparatorActive"		<< YAML::Value << m_Style.Colors[ImGuiCol_SeparatorActive];
		out << YAML::Key << "ImGuiCol_ResizeGrip"			<< YAML::Value << m_Style.Colors[ImGuiCol_ResizeGrip];
		out << YAML::Key << "ImGuiCol_ResizeGripHovered"	<< YAML::Value << m_Style.Colors[ImGuiCol_ResizeGripHovered];
		out << YAML::Key << "ImGuiCol_ResizeGripActive"		<< YAML::Value << m_Style.Colors[ImGuiCol_ResizeGripActive];
		out << YAML::Key << "ImGuiCol_Tab"					<< YAML::Value << m_Style.Colors[ImGuiCol_Tab];
		out << YAML::Key << "ImGuiCol_TabHovered"			<< YAML::Value << m_Style.Colors[ImGuiCol_TabHovered];
		out << YAML::Key << "ImGuiCol_TabActive"			<< YAML::Value << m_Style.Colors[ImGuiCol_TabActive];
		out << YAML::Key << "ImGuiCol_TabUnfocused"			<< YAML::Value << m_Style.Colors[ImGuiCol_TabUnfocused];
		out << YAML::Key << "ImGuiCol_TabUnfocusedActive"	<< YAML::Value << m_Style.Colors[ImGuiCol_TabUnfocusedActive];
		out << YAML::Key << "ImGuiCol_DockingPreview"		<< YAML::Value << m_Style.Colors[ImGuiCol_DockingPreview];
		out << YAML::Key << "ImGuiCol_DockingEmptyBg"		<< YAML::Value << m_Style.Colors[ImGuiCol_DockingEmptyBg];
		out << YAML::Key << "ImGuiCol_PlotLines"			<< YAML::Value << m_Style.Colors[ImGuiCol_PlotLines];
		out << YAML::Key << "ImGuiCol_PlotLinesHovered"		<< YAML::Value << m_Style.Colors[ImGuiCol_PlotLinesHovered];
		out << YAML::Key << "ImGuiCol_PlotHistogram"		<< YAML::Value << m_Style.Colors[ImGuiCol_PlotHistogram];
		out << YAML::Key << "ImGuiCol_PlotHistogramHovered"	<< YAML::Value << m_Style.Colors[ImGuiCol_PlotHistogramHovered];
		out << YAML::Key << "ImGuiCol_TableHeaderBg"		<< YAML::Value << m_Style.Colors[ImGuiCol_TableHeaderBg];
		out << YAML::Key << "ImGuiCol_TableBorderStrong"	<< YAML::Value << m_Style.Colors[ImGuiCol_TableBorderStrong];
		out << YAML::Key << "ImGuiCol_TableBorderLight"		<< YAML::Value << m_Style.Colors[ImGuiCol_TableBorderLight];
		out << YAML::Key << "ImGuiCol_TableRowBg"			<< YAML::Value << m_Style.Colors[ImGuiCol_TableRowBg];
		out << YAML::Key << "ImGuiCol_TableRowBgAlt"		<< YAML::Value << m_Style.Colors[ImGuiCol_TableRowBgAlt];
		out << YAML::Key << "ImGuiCol_TextSelectedBg"		<< YAML::Value << m_Style.Colors[ImGuiCol_TextSelectedBg];
		out << YAML::Key << "ImGuiCol_DragDropTarget"		<< YAML::Value << m_Style.Colors[ImGuiCol_DragDropTarget];
		out << YAML::Key << "ImGuiCol_NavHighlight"			<< YAML::Value << m_Style.Colors[ImGuiCol_NavHighlight];
		out << YAML::Key << "ImGuiCol_NavWindowingHighlight"<< YAML::Value << m_Style.Colors[ImGuiCol_NavWindowingHighlight];
		out << YAML::Key << "ImGuiCol_NavWindowingDimBg"	<< YAML::Value << m_Style.Colors[ImGuiCol_NavWindowingDimBg];
		out << YAML::Key << "ImGuiCol_ModalWindowDimBg"		<< YAML::Value << m_Style.Colors[ImGuiCol_ModalWindowDimBg];



		out << YAML::EndMap; // Colors
		out << YAML::EndMap; //

		std::ofstream fout(filepath);
		fout << out.c_str();

	}
	
	bool ImGuiStyleSerializer::Deserialize(const std::string& filepath)
	{
		std::ifstream stream(filepath);
		std::stringstream strStream;

		strStream << stream.rdbuf();

		YAML::Node data = YAML::Load(strStream.str());

		if (!data["THEME"])
			return false;

		std::string themeName = data["THEME"].as<std::string>();
		OAK_CORE_TRACE("Loading theme '{0}'", themeName);

		if (data["STYLE"])
		{
			auto style = data["STYLE"];

			m_Style.Alpha =						style["Alpha"].as<float>();
			m_Style.DisabledAlpha =				style["DisabledAlpha"].as<float>();
			m_Style.WindowPadding =				style["WindowPadding"].as<ImVec2>();
			m_Style.WindowRounding =			style["WindowRounding"].as<float>();
			m_Style.WindowBorderSize =			style["WindowBorderSize"].as<float>();
			m_Style.WindowMinSize =				style["WindowMinSize"].as<ImVec2>();
			m_Style.WindowTitleAlign =			style["WindowTitleAlign"].as<ImVec2>();
			m_Style.WindowMenuButtonPosition =	style["WindowMenuButtonPosition"].as<ImGuiDir>();
			m_Style.ChildRounding =				style["ChildRounding"].as<float>();
			m_Style.ChildBorderSize =			style["ChildBorderSize"].as<float>();
			m_Style.PopupRounding =				style["PopupRounding"].as<float>();
			m_Style.PopupBorderSize =			style["PopupBorderSize"].as<float>();
			m_Style.FramePadding =				style["FramePadding"].as<ImVec2>();
			m_Style.FrameRounding =				style["FrameRounding"].as<float>();
			m_Style.FrameBorderSize =			style["FrameBorderSize"].as<float>();
			m_Style.ItemSpacing =				style["ItemSpacing"].as<ImVec2>();
			m_Style.ItemInnerSpacing =			style["ItemInnerSpacing"].as<ImVec2>();
			m_Style.CellPadding =				style["CellPadding"].as<ImVec2>();
			m_Style.TouchExtraPadding =			style["TouchExtraPadding"].as<ImVec2>();
			m_Style.IndentSpacing =				style["IndentSpacing"].as<float>();
			m_Style.ColumnsMinSpacing =			style["ColumnsMinSpacing"].as<float>();
			m_Style.ScrollbarSize =				style["ScrollbarSize"].as<float>();
			m_Style.ScrollbarRounding =			style["ScrollbarRounding"].as<float>();
			m_Style.GrabMinSize =				style["GrabMinSize"].as<float>();
			m_Style.GrabRounding =				style["GrabRounding"].as<float>();
			m_Style.LayoutAlign =				style["LayoutAlign"].as<float>();
			m_Style.LogSliderDeadzone =			style["LogSliderDeadzone"].as<float>();
			m_Style.TabRounding =				style["TabRounding"].as<float>();
			m_Style.TabBorderSize =				style["TabBorderSize"].as<float>();
			m_Style.TabMinWidthForCloseButton = style["TabMinWidthForCloseButton"].as<float>();
			m_Style.ColorButtonPosition =		style["ColorButtonPosition"].as<ImGuiDir>();
			m_Style.ButtonTextAlign =			style["ButtonTextAlign"].as<ImVec2>();
			m_Style.SelectableTextAlign =		style["SelectableTextAlign"].as<ImVec2>();
			m_Style.DisplayWindowPadding =		style["DisplayWindowPadding"].as<ImVec2>();
			m_Style.DisplaySafeAreaPadding =	style["DisplaySafeAreaPadding"].as<ImVec2>();
			m_Style.MouseCursorScale =			style["MouseCursorScale"].as<float>();
			m_Style.AntiAliasedLines =			style["AntiAliasedLines"].as<bool>();
			m_Style.AntiAliasedLinesUseTex =	style["AntiAliasedLinesUseTex"].as<bool>();
			m_Style.AntiAliasedFill =			style["AntiAliasedFill"].as<bool>();
			m_Style.CurveTessellationTol =		style["CurveTessellationTol"].as<float>();
			m_Style.CircleTessellationMaxError =style["CircleTessellationMaxError"].as<float>();

			OAK_WARN(" window padding {}, {}", m_Style.WindowPadding.x, m_Style.WindowPadding.y);
			OAK_WARN(" tab rounding {}", m_Style.TabRounding);
		}
		if (data["Colors"])
		{
			auto colors = data["COLORS"];

			m_Style.Colors[ImGuiCol_Text] = colors["ImGuiCol_Text"].as<ImVec4>();
			m_Style.Colors[ImGuiCol_TextDisabled] = colors["ImGuiCol_TextDisabled"].as<ImVec4>();
			m_Style.Colors[ImGuiCol_WindowBg] = colors["ImGuiCol_WindowBg"].as<ImVec4>();
			m_Style.Colors[ImGuiCol_ChildBg] = colors["ImGuiCol_ChildBg"].as<ImVec4>();
			m_Style.Colors[ImGuiCol_PopupBg] = colors["ImGuiCol_PopupBg"].as<ImVec4>();
			m_Style.Colors[ImGuiCol_Border] = colors["ImGuiCol_Border"].as<ImVec4>();
			m_Style.Colors[ImGuiCol_BorderShadow] = colors["ImGuiCol_BorderShadow"].as<ImVec4>();
			m_Style.Colors[ImGuiCol_FrameBg] = colors["ImGuiCol_FrameBg"].as<ImVec4>();
			m_Style.Colors[ImGuiCol_FrameBgHovered] = colors["ImGuiCol_FrameBgHovered"].as<ImVec4>();
			m_Style.Colors[ImGuiCol_FrameBgActive] = colors["ImGuiCol_FrameBgActive"].as<ImVec4>();
			m_Style.Colors[ImGuiCol_TitleBg] = colors["ImGuiCol_TitleBg"].as<ImVec4>();
			m_Style.Colors[ImGuiCol_TitleBgActive] = colors["ImGuiCol_TitleBgActive"].as<ImVec4>();
			m_Style.Colors[ImGuiCol_TitleBgCollapsed] = colors["ImGuiCol_TitleBgCollapsed"].as<ImVec4>();
			m_Style.Colors[ImGuiCol_MenuBarBg] = colors["ImGuiCol_MenuBarBg"].as<ImVec4>();
			m_Style.Colors[ImGuiCol_ScrollbarBg] = colors["ImGuiCol_ScrollbarBg"].as<ImVec4>();
			m_Style.Colors[ImGuiCol_ScrollbarGrab] = colors["ImGuiCol_ScrollbarGrab"].as<ImVec4>();
			m_Style.Colors[ImGuiCol_ScrollbarGrabHovered] = colors["ImGuiCol_ScrollbarGrabHovered"].as<ImVec4>();
			m_Style.Colors[ImGuiCol_ScrollbarGrabActive] = colors["ImGuiCol_ScrollbarGrabActive"].as<ImVec4>();
			m_Style.Colors[ImGuiCol_CheckMark] = colors["ImGuiCol_CheckMark"].as<ImVec4>();
			m_Style.Colors[ImGuiCol_SliderGrab] = colors["ImGuiCol_SliderGrab"].as<ImVec4>();
			m_Style.Colors[ImGuiCol_SliderGrabActive] = colors["ImGuiCol_SliderGrabActive"].as<ImVec4>();
			m_Style.Colors[ImGuiCol_Button] = colors["ImGuiCol_Button"].as<ImVec4>();
			m_Style.Colors[ImGuiCol_ButtonHovered] = colors["ImGuiCol_ButtonHovered"].as<ImVec4>();
			m_Style.Colors[ImGuiCol_ButtonActive] = colors["ImGuiCol_ButtonActive"].as<ImVec4>();
			m_Style.Colors[ImGuiCol_Header] = colors["ImGuiCol_Header"].as<ImVec4>();
			m_Style.Colors[ImGuiCol_HeaderHovered] = colors["ImGuiCol_HeaderHovered"].as<ImVec4>();
			m_Style.Colors[ImGuiCol_HeaderActive] = colors["ImGuiCol_HeaderActive"].as<ImVec4>();
			m_Style.Colors[ImGuiCol_Separator] = colors["ImGuiCol_Separator"].as<ImVec4>();
			m_Style.Colors[ImGuiCol_SeparatorHovered] = colors["ImGuiCol_SeparatorHovered"].as<ImVec4>();
			m_Style.Colors[ImGuiCol_SeparatorActive] = colors["ImGuiCol_SeparatorActive"].as<ImVec4>();
			m_Style.Colors[ImGuiCol_ResizeGrip] = colors["ImGuiCol_ResizeGrip"].as<ImVec4>();
			m_Style.Colors[ImGuiCol_ResizeGripHovered] = colors["ImGuiCol_ResizeGripHovered"].as<ImVec4>();
			m_Style.Colors[ImGuiCol_ResizeGripActive] = colors["ImGuiCol_ResizeGripActive"].as<ImVec4>();
			m_Style.Colors[ImGuiCol_Tab] = colors["ImGuiCol_Tab"].as<ImVec4>();
			m_Style.Colors[ImGuiCol_TabHovered] = colors["ImGuiCol_TabHovered"].as<ImVec4>();
			m_Style.Colors[ImGuiCol_TabActive] = colors["ImGuiCol_TabActive"].as<ImVec4>();
			m_Style.Colors[ImGuiCol_TabUnfocused] = colors["ImGuiCol_TabUnfocused"].as<ImVec4>();
			m_Style.Colors[ImGuiCol_TabUnfocusedActive] = colors["ImGuiCol_TabUnfocusedActive"].as<ImVec4>();
			m_Style.Colors[ImGuiCol_DockingPreview] = colors["ImGuiCol_DockingPreview"].as<ImVec4>();
			m_Style.Colors[ImGuiCol_DockingEmptyBg] = colors["ImGuiCol_DockingEmptyBg"].as<ImVec4>();
			m_Style.Colors[ImGuiCol_PlotLines] = colors["ImGuiCol_PlotLines"].as<ImVec4>();
			m_Style.Colors[ImGuiCol_PlotLinesHovered] = colors["ImGuiCol_PlotLinesHovered"].as<ImVec4>();
			m_Style.Colors[ImGuiCol_PlotHistogram] = colors["ImGuiCol_PlotHistogram"].as<ImVec4>();
			m_Style.Colors[ImGuiCol_PlotHistogramHovered] = colors["ImGuiCol_PlotHistogramHovered"].as<ImVec4>();
			m_Style.Colors[ImGuiCol_TableHeaderBg] = colors["ImGuiCol_TableHeaderBg"].as<ImVec4>();
			m_Style.Colors[ImGuiCol_TableBorderStrong] = colors["ImGuiCol_TableBorderStrong"].as<ImVec4>();
			m_Style.Colors[ImGuiCol_TableBorderLight] = colors["ImGuiCol_TableBorderLight"].as<ImVec4>();
			m_Style.Colors[ImGuiCol_TableRowBg] = colors["ImGuiCol_TableRowBg"].as<ImVec4>();
			m_Style.Colors[ImGuiCol_TableRowBgAlt] = colors["ImGuiCol_TableRowBgAlt"].as<ImVec4>();
			m_Style.Colors[ImGuiCol_TextSelectedBg] = colors["ImGuiCol_TextSelectedBg"].as<ImVec4>();
			m_Style.Colors[ImGuiCol_DragDropTarget] = colors["ImGuiCol_DragDropTarget"].as<ImVec4>();
			m_Style.Colors[ImGuiCol_NavHighlight] = colors["ImGuiCol_NavHighlight"].as<ImVec4>();
			m_Style.Colors[ImGuiCol_NavWindowingHighlight] = colors["ImGuiCol_NavWindowingHighlight"].as<ImVec4>();
			m_Style.Colors[ImGuiCol_NavWindowingDimBg] = colors["ImGuiCol_NavWindowingDimBg"].as<ImVec4>();
			m_Style.Colors[ImGuiCol_ModalWindowDimBg] = colors["ImGuiCol_ModalWindowDimBg"].as<ImVec4>();
		}


		return true;
	}
	
}
