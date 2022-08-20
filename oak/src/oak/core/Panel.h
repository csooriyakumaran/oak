#pragma once
#include "oakpch.h"

namespace Oak
{
    class Panel
    {
    public:
        Panel(const std::string& name, bool open);
        virtual ~Panel() = default;
        
        virtual void SetLocalStyle() {}
        virtual void OnUIRender(bool* open) {}

        void Show() { m_open = true; }
        void Hide() { m_open = false; }
        void Toggle() { m_open = !m_open; }
        bool Visibility() { return m_open; }
        
        const std::string GetName() { return m_Name; }
        uint32_t GetID() const { return m_id; }
        
        bool* p_open = &m_open;

    private:
        uint32_t m_id;
        bool m_open = false;
        inline static uint32_t idGenerator = 0;
        
        std::string m_Name = "Panel";

    };

}
