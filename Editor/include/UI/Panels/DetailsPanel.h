//
// Created by Oskar.Norberg on 2025-11-13.
//

#pragma once

#include "DetailsSubPanel/DetailsSubPanel.h"
#include "DetailsSubPanel/SubPanelManager.h"
#include "Scene/SceneManager/SceneManager.h"
#include "UI/IDockablePanel.h"
#include "UI/PanelInitializer.h"

namespace RNGOEngine::Editor
{
    class DetailsPanel : public IDockablePanel
    {
    public:
        void Render(UIContext& context) override;

        std::string_view GetPanelName() const override
        {
            return "Details";
        }

    private:
        SubPanelManager m_subPanelManager{};

    private:
        void UpdateSubPanelBasedOnSelection(UIContext& context);
    };
    EDITOR_PANEL(DetailsPanel);
}