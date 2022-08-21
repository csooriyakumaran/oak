#pragma once
#include "oakpch.h"
#include "oak/core/Panel.h"

namespace Oak
{
        
    class PanelStack
    {
    public:
        PanelStack() = default;
        ~PanelStack();

        void PushPanel(Panel* panel);
        void PopPanel(Panel* panel);
        void ClearStack();
      
        std::vector<Panel*> GetPanels() {return m_Panels; }

        std::vector<Panel*>::iterator begin() { return m_Panels.begin(); }
        std::vector<Panel*>::iterator end() { return m_Panels.end(); }
        std::vector<Panel*>::reverse_iterator rbegin() { return m_Panels.rbegin(); }
        std::vector<Panel*>::reverse_iterator rend() { return m_Panels.rend(); }
    
        std::vector<Panel*>::const_iterator begin() const { return m_Panels.begin(); }
        std::vector<Panel*>::const_iterator end()	const { return m_Panels.end(); }
        std::vector<Panel*>::const_reverse_iterator rbegin() const { return m_Panels.rbegin(); }
        std::vector<Panel*>::const_reverse_iterator rend() const { return m_Panels.rend(); }

    private:
        std::vector<Panel*> m_Panels;
        unsigned int m_PanelInsertIndex = 0;
    };

}