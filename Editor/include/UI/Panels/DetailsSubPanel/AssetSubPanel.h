//
// Created by Oskar.Norberg on 2026-02-09.
//

#pragma once

#include "DetailsSubPanel.h"
#include "SubPanelManager.h"

namespace RNGOEngine::Editor
{
    class AssetDetailsSubPanel : public DetailsSubPanel
    {
    public:
        void RenderSubPanel(UIContext& context) override;

        std::string_view GetSubPanelName() const override
        {
            return "Asset Details";
        }

    private:
        void SwitchSubPanelBasedOnAsset(const AssetHandling::AssetHandle& assetHandle, UIContext& context);

    private:
        SubPanelManager m_subPanelManager{};
    };
}