#pragma once

#include "oakpch.h"

#include "imgui.h"

namespace Oak
{

    //enum class PinType
    //{
    //    Flow,
    //    Bool,
    //    Int,
    //    Float,
    //    String,
    //    Object,
    //    Function
    //};

    //enum class PinKind
    //{
    //    Output,
    //    Input
    //};

    //enum class StorageKind
    //{
    //    Value = 0,
    //    Array
    //};

    //enum class NodeType
    //{
    //    Simple,
    //    Comment,
    //    Operator,
    //    Clock,
    //    Plot, 

    //};


    //template<typename T>
    //struct Pin
    //{
    //    UUID ID;
    //    UUID NodeID;
    //    std::string Name;
    //    PinType Type;
    //    PinKind Kind;
    //    StorageKind Storage;
    //    T Value;

    //    Pin(UUID id, const char* name, PinType type, PinKind kind, StorageKind storageKind, T value)
    //        : ID(id), Name(name), Type(type), PinKind(kind), Storage(storageKind)
    //    {
    //    }

    //};

    //template <typename T>
    //struct Node
    //{
    //    UUID ID;
    //    std::string Name;
    //    std::vector<Pin<T>> Inputs;
    //    std::vector<Pin<T>> Outputs;
    //    NodeType Type;
    //    ImVec2 Size;

    //    Node(UUID id, const char* name)
    //        : ID(id), Name(name)
    //    {
    //    }

    //    bool operator==(const Node& other) const { return ID == other.ID; }
    //    
    //};
  
    class Node
    {
    public:
        Node(bool open) : m_open(open), uuid(wi::UUID()) {}
        virtual ~Node() = default;
        
        virtual void SetLocalStyle() {}
        virtual void OnUIRender(bool* open) {}

        void Show() { m_open = true; }
        void Hide() { m_open = false; }
        void Toggle() { m_open = !m_open; }
        bool Visibility() { return m_open; }
        
        void SetName(const std::string& name) { m_Name = name; }
        const std::string GetName() { return m_Name; }
        wi::UUID GetID() const { return uuid; }
        
        bool* p_open = &m_open;

    private:
        wi::UUID uuid;
        bool m_open = false;
        
        std::string m_Name = "Node";

    };

}
