#pragma once
#include "oakpch.h"
#include "oak/core/Base.h"
#include <imgui.h>


namespace Oak
{
    class Panel : public RefCounted
    {
    public:
        
        //Panel(const std::string& name, bool open);
        virtual ~Panel(){};
        
        //virtual void SetLocalStyle() {}
        virtual void OnUIRender(const char* name, bool& open) {}
        virtual void OnEvent(Event& e) {}
        virtual void OnProjectChanged() {}
        
        
        virtual void PushLocalStyle() {}
        
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
