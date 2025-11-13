//
// Created by Oskar.Norberg on 2025-11-13.
//

#pragma once
#include "Scene/SceneManager/SceneManager.h"
#include "UI/IDockablePanel.h"

namespace RNGOEngine::Editor
{
    class DetailsPanel : public IDockablePanel
    {
    public:
        // TODO: Again, accessing the SceneManager directly like this feels wrong. Consider adding it to the UIContext?
        explicit DetailsPanel(Core::SceneManager& sceneManager);
        
        void Render(UIContext& context) override;

        std::string_view GetPanelName() const override
        {
            return "Details";
        }

    private:
        Core::SceneManager& m_sceneManager;
    };
}