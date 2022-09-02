#pragma once
#include "oakpch.h"
#include "oak/core/Base.h"
#include <imgui.h>

// include Yamlcpp

namespace Oak
{
	class ImGuiStyleSerializer
	{
	public:
		ImGuiStyleSerializer(ImGuiStyle& style);

		void Serialize(const std::string& filepath);
		bool Deserialize(const std::string& filepath);

	private:
		ImGuiStyle& m_Style;
	};


}