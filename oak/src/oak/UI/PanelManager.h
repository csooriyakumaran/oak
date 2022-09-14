#pragma once

#include "oak/core/Base.h"
#include "oak/core/Log.h"
#include "oak/core/Hash.h"

#include "oak/UI/Panel.h"

#include <unordered_map>

namespace Oak
{
    struct PanelData
    {
        const char* ID = "";
        const char* Name = "";
        Ref<Panel> Panel = nullptr;
        bool IsOpen = false;
    };

    // for now to determine which menu the panel will be listed in (in the application)
    enum class PanelCategory
    {
        VIEW, EDIT, TOOLS, HELP, _COUNT
    };

    class PanelManager
    {
    public:
        PanelManager() = default;
        ~PanelManager()
        {
            for (auto& map : m_Panels)
                map.clear();
        }

        template<typename TPanel>
        Ref<TPanel> AddPanel(PanelCategory category, const PanelData& panelData)
        {
            static_assert(std::is_base_of<Panel, TPanel>::value, "PanelManager::AddPanel requires TPanel to inherit from Oak::Panel");

            auto& panelMap = m_Panels[(size_t)category];

            uint32_t id = Hash::GenerateFNVHash(panelData.ID);
            if (panelMap.find(id) != panelMap.end())
            {
                OAK_CORE_ERROR_TAG("PannelManager::AddPanel", "A panel with id '{0}' has already been added.", panelData.ID);
                return nullptr;
            }

            panelMap[id] = panelData;
            return panelData.Panel.As<TPanel>();

        }

        template<typename TPanel, typename... TArgs>
        Ref<TPanel> AddPanel(PanelCategory category, const char* strID, bool isOpenByDefault, TArgs&&... args)
        {
            return AddPanel<TPanel>(category, PanelData{ strID, strID, Ref<TPanel>::Create(std::forward<TArgs>(args)...), isOpenByDefault });
        }

        template<typename TPanel, typename... TArgs>
        Ref<TPanel> AddPanel(PanelCategory category, const char* strID, const char* displayName, bool isOpenByDefault, TArgs&&... args)
        {
            return AddPanel<TPanel>(category, PanelData{ strID, displayName, Ref<TPanel>::Create(std::forward<TArgs>(args)...), isOpenByDefault });
        }

        template<typename TPanel>
        Ref<TPanel> GetPanel(const char* strID)
        {
            static_assert(std::is_base_of<Panel, TPanel>::value, "PanelManager::GetPanel requires TPanel to inherit from Oak::Panel");
            
            uint32_t id = Hash::GenerateFNVHash(strID);
            for (const auto& panelMap : m_Panels)
            {
                if (panelMap.find(id) == panelMap.end())
                    continue;
                return panelMap.at(id).Panel.As<TPanel>();
            }
            OAK_CORE_ERROR_TAG("PanelManager::GetPanel", "Couldn't find panel with id '{0}'", strID);
            return nullptr;
        }

        const char* GetPanelName(const char* strID)
        {
            uint32_t id = Hash::GenerateFNVHash(strID);
            for (const auto& panelMap : m_Panels)
            {
                if (panelMap.find(id) == panelMap.end())
                    continue;
                return panelMap.at(id).Name;
            }
            OAK_CORE_ERROR_TAG("PanelManager::GetPanelName", "Could not fine panel with id {}", strID);
            return nullptr;
        }

        bool IsPanelOpen(const char* strID)
        {
            uint32_t id = Hash::GenerateFNVHash(strID);
            for (const auto& panelMap : m_Panels)
            {
                if (panelMap.find(id) == panelMap.end())
                    continue;
                return panelMap.at(id).IsOpen;
            }
        }

        void TogglePanel(const char* strID)
        {
            uint32_t id = Hash::GenerateFNVHash(strID);
            for (auto& panelMap : m_Panels)
            {
                if (panelMap.find(id) == panelMap.end())
                    continue;

                panelMap.at(id).IsOpen ^= true;
                return;
            }
        }

        void RemovePanel(const char* strID)
        {
            uint32_t id = Hash::GenerateFNVHash(strID);
            for (auto& panelMap : m_Panels)
            {
                if (panelMap.find(id) == panelMap.end())
                    continue;

                panelMap.erase(id);
                return;
            }

            OAK_CORE_ERROR_TAG("PanelManager::RemovePanel", "Couldn't find panel with id '{0}'", strID);
        }

        void OnUIRender()
        {
            for (auto& panelMap : m_Panels)
            {
                for (auto& [id, panelData] : panelMap)
                {
                    if (panelData.IsOpen)
                    {   
                        panelData.Panel->PushLocalStyle();
                        panelData.Panel->OnUIRender(panelData.Name, panelData.IsOpen);
                        panelData.Panel->PopLocalStyle();
                    }
                }
            }
        }

        void OnEvent(Event& e)
        {
            for (auto& panelMap : m_Panels)
            {
                for (auto& [id, panelData] : panelMap)
                    panelData.Panel->OnEvent(e);
            }
        }

        //void OnProjectChanged(const Ref<Project>& project)
        //{
        //    for (auto& panelMap : m_Panels)
        //    {
        //        for (auto& [id, panelData] : panelMap)
        //            panelData.Panel->OnProjectChanged(project);
        //    }
        //}

        std::unordered_map<uint32_t, PanelData>& GetPanels(PanelCategory category) { return m_Panels[(size_t)category]; }
        const std::unordered_map<uint32_t, PanelData>& GetPanels(PanelCategory category) const { return m_Panels[(size_t)category]; }

    private:
        std::array<std::unordered_map<uint32_t, PanelData>, (size_t)PanelCategory::_COUNT> m_Panels;


    };
}