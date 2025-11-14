//
// Created by Oskar.Norberg on 2025-11-13.
//

#pragma once

#include "UI/Managers/UISelectionManager.h"

namespace RNGOEngine::Editor
{
    void UISelectionManager::SetSelectedEntity(const entt::entity entity)
    {
        m_selectedEntity = entity;
    }

    entt::entity UISelectionManager::GetSelectedEntity() const
    {
        return m_selectedEntity;
    }
}