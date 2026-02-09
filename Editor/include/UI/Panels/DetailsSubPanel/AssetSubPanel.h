//
// Created by Oskar.Norberg on 2026-02-09.
//

#pragma once
#include "DetailsSubPanel.h"

namespace RNGOEngine::Editor
{
    class AssetDetailsSubPanel : public DetailsSubPanel
    {
    public:
        void RenderSubPanel(UIContext& context) override
        {
            const auto currentSelection = context.selectionManager.GetCurrentSelection();
            if (!std::holds_alternative<AssetSelection>(currentSelection))
            {
                ImGui::Text("No asset selected.");
                return;
            }
            const auto& selectedAssetHandle = std::get<AssetSelection>(currentSelection).AssetHandle;

            ImGui::Text("Selected Asset: %llu", selectedAssetHandle.UUID.GetValue());
        }

        std::string_view GetSubPanelName() const override
        {
            return "Asset Details";
        }
    };
}