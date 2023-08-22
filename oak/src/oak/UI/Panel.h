#pragma once
#include "oakpch.h"
#include "oak/core/Base.h"
#include "oak/events/Event.h"
#include <imgui.h>

#include "wi.h"

namespace Oak
{
    class Panel : public wi::RefCounted
    {
    public:
        Panel() {};
        ~Panel(){};
        virtual void PushLocalStyle() {}
        virtual void OnUIRender(const char* name, bool& open) {}
        virtual void OnEvent(Event& e) {}
        virtual void OnProjectChanged() {}

        void PopLocalStyle() 
        {
            if (m_StylePopCount > 0)
            {
                ImGui::PopStyleVar(m_StylePopCount);
                m_StylePopCount = 0;
            }

            if (m_ColorPopCount > 0)
            {
                ImGui::PopStyleColor(m_ColorPopCount);
                m_ColorPopCount = 0;
            }
        }

    protected:
        uint32_t m_StylePopCount = 0;
        uint32_t m_ColorPopCount = 0;
    };
}
