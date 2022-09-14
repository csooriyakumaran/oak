#pragma once
#include "oakpch.h"
#include "oak/renderer/Texture.h"
#include "oak/renderer/RendererAPI.h"
#include "oak/renderer/Renderer.h"
#include "oak/platform/OpenGL/OpenGLTexture.h"

#include "imgui.h"

#ifndef IMGUI_DEFINE_MATH_OPERATORS
#define IMGUI_DEFINE_MATH_OPERATORS
#endif
#include "imgui_internal.h"

namespace Oak
{
   template <typename TTexture>
   bool ImageButton(Ref<TTexture> tex, ImVec2& size, int pad=0)
   {
	   static_assert(std::is_base_of<Texture, TTexture>::value, "Oak::ImageButton requires TTexture to inherit from Oak::Texture");
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None:
			{
				OAK_CORE_ASSERT(false, "RendererAPI::None is currently not supported for Oak::ImageButton!"); 
				return false;
			}
			case RendererAPI::API::OpenGL:
			{
				ImGuiContext* g = ImGui::GetCurrentContext();
				ImGuiWindow* window = g->CurrentWindow;
				if (window->SkipItems)
					return false;

				ImTextureID user_texture_id = (ImTextureID)tex->GetRendererID();
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

				window->DrawList->AddImage(user_texture_id, bb.Min + padding, bb.Max - padding, ImVec2(0,0), ImVec2(1,1), ImGui::GetColorU32(tint_col));

				return pressed;
			}
		}
		OAK_CORE_ASSERT(false, "Unknown RendererAPI!");
		return false;

        
   }

}