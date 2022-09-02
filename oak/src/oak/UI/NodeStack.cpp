#include "oakpch.h"
#include "NodeStack.h"

namespace Oak
{
    NodeStack::~NodeStack()
    {
        for (Node* node : m_Nodes)
        {
            delete node;
        }
    }

    void NodeStack::PushNode(Node* node)
    {
        m_Nodes.emplace(m_Nodes.begin() + m_NodeInsertIndex, node);
        m_NodeInsertIndex++;
    }
	void NodeStack::PopNode(Node* node)
	{
		auto it = std::find(m_Nodes.begin(), m_Nodes.begin() + m_NodeInsertIndex, node);
		if (it != m_Nodes.begin() + m_NodeInsertIndex)
		{
			m_Nodes.erase(it);
			m_NodeInsertIndex--;
		}
	}

    void NodeStack::ClearStack()
    {
        m_Nodes.clear();
        m_NodeInsertIndex = 0;
    }

}