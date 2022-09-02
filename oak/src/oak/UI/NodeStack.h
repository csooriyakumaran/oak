#pragma once
#include "oakpch.h"
#include "Node.h"

namespace Oak
{
        
    class NodeStack
    {
    public:
        NodeStack() = default;
        ~NodeStack();

        void PushNode(Node* Node);
        void PopNode(Node* Node);
        void ClearStack();
      
        std::vector<Node*> GetNodes() {return m_Nodes; }

        std::vector<Node*>::iterator begin() { return m_Nodes.begin(); }
        std::vector<Node*>::iterator end() { return m_Nodes.end(); }
        std::vector<Node*>::reverse_iterator rbegin() { return m_Nodes.rbegin(); }
        std::vector<Node*>::reverse_iterator rend() { return m_Nodes.rend(); }
    
        std::vector<Node*>::const_iterator begin() const { return m_Nodes.begin(); }
        std::vector<Node*>::const_iterator end()	const { return m_Nodes.end(); }
        std::vector<Node*>::const_reverse_iterator rbegin() const { return m_Nodes.rbegin(); }
        std::vector<Node*>::const_reverse_iterator rend() const { return m_Nodes.rend(); }

    private:
        std::vector<Node*> m_Nodes;
        unsigned int m_NodeInsertIndex = 0;
    };

}