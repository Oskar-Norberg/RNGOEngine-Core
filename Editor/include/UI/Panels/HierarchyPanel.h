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
        // TODO: Idk if this panel should access the SceneManager directly.
        explicit HierarchyPanel(Core::SceneManager& sceneManager);

    public:
        void Render() override;

        std::string_view GetPanelName() const override
        {
            return "Hierarchy";
        }

    private:
        Core::SceneManager& m_sceneManager;
    };
}