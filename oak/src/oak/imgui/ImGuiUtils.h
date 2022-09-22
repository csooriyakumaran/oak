#pragma once

#include "oakpch.h"
#include "oak/core/Ref.h"
#include "oak/renderer/RendererAPI.h"
#include "oak/renderer/Texture.h"
#include "oak/platform/OpenGL/OpenGLTexture.h"

#include "imgui.h"

namespace Oak::UI::Utils
{
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
                OAK_CORE_ERROR_TAG("UI::Utils::GetTextreID", "RendereAPI::None is not supported");
                return (ImTextureID)0;
            }
            default:
            {
                OAK_CORE_ERROR_TAG("UI::Utils::GetTextreID", "Unkown RendererAPI");
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


}