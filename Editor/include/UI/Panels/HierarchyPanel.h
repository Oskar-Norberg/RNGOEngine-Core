//
// Created by Oskar.Norberg on 2025-11-10.
//

#pragma once

#include <Scene/SceneManager/SceneManager.h>

#include "UI/IDockablePanel.h"

namespace RNGOEngine::Editor
{
    class HierarchyPanel : public IDockablePanel
    {
    public:
        void Render(UIContext& context) override;

        std::string_view GetPanelName() const override
        {
            return "Hierarchy";
        }
    };
}