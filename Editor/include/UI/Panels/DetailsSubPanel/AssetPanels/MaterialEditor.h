//
// Created by Oskar.Norberg on 2026-02-09.
//

#pragma once

#include "UI/Panels/DetailsSubPanel/DetailsSubPanel.h"

namespace RNGOEngine::AssetHandling
{
    class MaterialAsset;
}
namespace RNGOEngine::Editor
{
    class MaterialEditor : public DetailsSubPanel
    {
    public:
        void RenderSubPanel(UIContext& context) override;

        std::string_view GetSubPanelName() const override
        {
            return "Material Editor";
        }

    private:
        std::optional<AssetHandling::AssetHandle> GetHandleFromSelection(const UISelectionManager& selectionManager);
        AssetHandling::MaterialAsset* GetMaterialAsset(const AssetHandling::AssetHandle& materialHandle);
    };
}