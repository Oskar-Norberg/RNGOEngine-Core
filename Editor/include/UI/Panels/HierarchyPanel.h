//
// Created by Oskar.Norberg on 2025-11-10.
//

#pragma once

#include <Scene/SceneManager/SceneManager.h>

#include "UI/IDockablePanel.h"
#include "UI/PanelInitializer.h"

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

        // Spawning of Specialized Entities
    private:
        void CreateBackgroundColorEntity(UIContext& context);
        void CreateCameraEntity(UIContext& context);
        void CreateAmbientLightEntity(UIContext& context);
        void CreateDirectionalLightEntity(UIContext& context);
        void CreatePointLightEntity(UIContext& context);
        void CreateSpotlightEntity(UIContext& context);
        void CreateMeshEntity(UIContext& context);
    };
    EDITOR_PANEL(HierarchyPanel);
}