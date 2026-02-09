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
        void RenderSubPanel(UIContext& context) override;

        std::string_view GetSubPanelName() const override
        {
            return "Asset Details";
        }
    };
}