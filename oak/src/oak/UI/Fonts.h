#pragma once

#include <string>
#include <map>
#include <unordered_map>

#include "oakpch.h"
#include "imgui.h"

namespace Oak
{
	namespace UI
	{
		static std::unordered_map<std::string, ImFont*> s_Fonts;

		const ImWchar* GetGlyphRangeGreek()
		{
			static const ImWchar ranges[] =
			{
				0x0020, 0x00FF, // Basic Latin + Latin Supplement
				0x0370, 0x03FF, // Greek
				0x0400, 0x052F, // Cyrillic + Cyrillic Supplement
				0x2DE0, 0x2DFF, // Cyrillic Extended-A
				0xA640, 0xA69F, // Cyrillic Extended-B
				0,
			};
			return &ranges[0];
		}
		
		std::string GenerateFontKey(std::string name, float size)
		{
			return name + std::to_string((int)size * 100);
		}

		struct Fonts
		{

			static void Add(const std::string& name, const std::string& filepath, float size, bool isDefault)
			{
				std::string key = GenerateFontKey(name, size);
				if (s_Fonts.find(key) != s_Fonts.end())
				{
					OAK_CORE_WARN_TAG("UI::Fonts", "Font with name {0} already exists with size {1}", name, size);
					return;
				}
				ImGuiIO& io = ImGui::GetIO();
				ImFont* font = io.Fonts->AddFontFromFileTTF(filepath.c_str(), size, nullptr, GetGlyphRangeGreek());
				OAK_CORE_VERIFY(font, "Faied to load font from file");
				s_Fonts[key] = font;

				if (isDefault)
					io.FontDefault = font;
			}

			static ImFont* Get(const std::string& name, float size)
			{
				std::string key = GenerateFontKey(name, size);
				if (s_Fonts.find(key) == s_Fonts.end())
				{
					OAK_CORE_ERROR_TAG("UI::Fonts", "No font named {} found in the font library. Add font from .ttf file", name);
					return nullptr;
				}
				return s_Fonts[key];
			}

		};
	}
}
