#pragma once

#include "oakpch.h"
#include "oak/renderer/RendererAPI.h"
#include "oak/renderer/Texture.h"
#include "oak/platform/OpenGL/OpenGLTexture.h"

#ifndef IMGUI_DEFINE_MATH_OPERATORS
#define IMGUI_DEFINE_MATH_OPERATORS
#endif
#include "imgui_internal.h"


namespace Oak::UI
{

    //bool BeginPopup(const char* str_id, ImGuiWindowFlags flags = 0);
    //void EndPopup();

    // MenuBar which allows you to specify its rectangle
    bool BeginMenuBar(const ImRect& barRectangle);
    void EndMenuBar();

    // Exposed to be used for window with disabled decorations
// This border is going to be drawn even if window border size is set to 0.0f
    void RenderWindowOuterBorders(ImGuiWindow* window);

    // Exposed resize behavior for native OS windows
    bool UpdateWindowManualResize(ImGuiWindow* window, ImVec2& newSize, ImVec2& newPosition);

    using namespace wi;
    inline ImTextureID GetImTextureID(Ref<Texture2D> texture)
    {
        switch (RendererAPI::GetAPI())
        {
            case RendererAPI::API::OpenGL:
            {
                Ref<OpenGLTexture2D> openGLTexture = texture.As<OpenGLTexture2D>();
                if(!openGLTexture->IsLoaded())
                    return nullptr;
                return (ImTextureID)openGLTexture->GetRendererID();
            }
            case RendererAPI::API::None:
            {
                LOG_CORE_ERROR_TAG("UI::Utils::GetTextreID", "RendereAPI::None is not supported");
                return (ImTextureID)0;
            }
            default:
            {
                LOG_CORE_ERROR_TAG("UI::Utils::GetTextreID", "Unkown RendererAPI");
                return nullptr;
            }

        }
    }

    class ScopedStyle
    {
    public:
        ScopedStyle(const ScopedStyle&) = delete;
        ScopedStyle& operator=(const ScopedStyle&) = delete;
        template<typename T>
        ScopedStyle(ImGuiStyleVar styleVar, T value) { ImGui::PushStyleVar(styleVar, value); }
        ~ScopedStyle() { ImGui::PopStyleVar(); }
    };

    class ScopedColour
    {
    public:
        ScopedColour(const ScopedColour&) = delete;
        ScopedColour& operator=(const ScopedColour&) = delete;
        template<typename T>
        ScopedColour(ImGuiCol colourId, T colour) { ImGui::PushStyleColor(colourId, ImColor(colour).Value); }
        ~ScopedColour() { ImGui::PopStyleColor(); }
    };

    class ScopedFont
    {
    public:
        ScopedFont(const ScopedFont&) = delete;
        ScopedFont& operator=(const ScopedFont&) = delete;
        ScopedFont(ImFont* font) { ImGui::PushFont(font); }
        ~ScopedFont() { ImGui::PopFont(); }
    };


}