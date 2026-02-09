//
// Created by Oskar.Norberg on 2025-11-13.
//

#include "UI/Managers/UISelectionManager.h"

namespace RNGOEngine::Editor
{
    void UISelectionManager::SelectEntity(const entt::entity entity)
    {
        m_currentSelection = EntitySelection{entity};
    }

    void UISelectionManager::SelectAsset(const AssetHandling::AssetHandle& assetHandle)
    {
        m_currentSelection = AssetSelection{assetHandle};
    }

    void UISelectionManager::Deselect()
    {
        m_currentSelection = NoSelection{};
    }

    SelectionVariant UISelectionManager::GetCurrentSelection() const
    {
        return m_currentSelection;
    }
}