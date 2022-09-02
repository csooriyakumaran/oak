#include "oakpch.h"
#include "PanelStack.h"

namespace Oak
{
    PanelStack::~PanelStack()
    {
        for (Panel* panel : m_Panels)
        {
            delete panel;
        }
    }

    void PanelStack::PushPanel(Panel* panel)
    {
        m_Panels.emplace(m_Panels.begin() + m_PanelInsertIndex, panel);
        m_PanelInsertIndex++;
    }
	void PanelStack::PopPanel(Panel* panel)
	{
		auto it = std::find(m_Panels.begin(), m_Panels.begin() + m_PanelInsertIndex, panel);
		if (it != m_Panels.begin() + m_PanelInsertIndex)
		{
			m_Panels.erase(it);
			m_PanelInsertIndex--;
		}
	}

    void PanelStack::ClearStack()
    {
        m_Panels.clear();
        m_PanelInsertIndex = 0;
    }

}