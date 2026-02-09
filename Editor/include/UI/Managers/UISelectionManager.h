//
// Created by Oskar.Norberg on 2025-11-13.
//

#pragma once

#include "Assets/Asset.h"
#include "entt/entt.hpp"

namespace RNGOEngine::AssetHandling
{
    struct AssetHandle;
}

namespace RNGOEngine::Editor
{
    struct NoSelection
    {
    };
    struct EntitySelection
    {
        entt::entity Entity;
    };
    struct AssetSelection
    {
        AssetHandling::AssetHandle AssetHandle;
    };

    using SelectionVariant = std::variant<NoSelection, EntitySelection, AssetSelection>;

    class UISelectionManager
    {
        // Selectors
    public:
        void SelectEntity(entt::entity entity);
        void SelectAsset(const AssetHandling::AssetHandle& assetHandle);

        void Deselect();

    public:
        SelectionVariant GetCurrentSelection() const;

    private:
        SelectionVariant m_currentSelection{NoSelection{}};
    };
}