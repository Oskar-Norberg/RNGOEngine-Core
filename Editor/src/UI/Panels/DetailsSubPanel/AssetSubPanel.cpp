//
// Created by Oskar.Norberg on 2026-02-09.
//

#include "UI/Panels/DetailsSubPanel/AssetSubPanel.h"

#include "Assets/AssetDatabase/AssetDatabase.h"
#include "magic_enum/magic_enum.hpp"

namespace RNGOEngine::Editor
{

    void AssetDetailsSubPanel::RenderSubPanel(UIContext& context)

    {
        const auto currentSelection = context.selectionManager.GetCurrentSelection();
        if (!std::holds_alternative<AssetSelection>(currentSelection))
        {
            ImGui::Text("No asset selected.");
            return;
        }

        const auto& selectedAssetHandle = std::get<AssetSelection>(currentSelection).AssetHandle;

        // Display asset details
        const auto& assetDatabase = AssetHandling::AssetDatabase::GetInstance();
        const auto assetInfoOpt = assetDatabase.TryGetAssetMetadata(selectedAssetHandle);

        if (!assetInfoOpt.has_value())
        {
            ImGui::Text("No asset selected or not registered.");
            return;
        }

        const auto assetFilename = assetInfoOpt->get().Path.filename().string();
        ImGui::Text("%s", assetFilename.c_str());
    }
}