//
// Created by Oskar.Norberg on 2026-01-26.
//

#pragma once

#include "UI/IDockablePanel.h"
#include "UI/PanelInitializer.h"

namespace RNGOEngine::Editor
{
    class SceneManagementPanel : public IDockablePanel
    {
    public:
        void Render(UIContext& context) override;

        std::string_view GetPanelName() const override
        {
            return "Scene Manager";
        }
    };
    EDITOR_PANEL(SceneManagementPanel);
}