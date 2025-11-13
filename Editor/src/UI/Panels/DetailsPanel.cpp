//
// Created by Oskar.Norberg on 2025-11-13.
//

#include "UI/Panels/DetailsPanel.h"

#include "UI/Managers/UISelectionManager.h"

namespace RNGOEngine::Editor
{
    DetailsPanel::DetailsPanel(Core::SceneManager& sceneManager)
        : m_sceneManager(sceneManager)
    {
    }

    void DetailsPanel::Render(UIContext& context)
    {
        IDockablePanel::Render(context);

        auto& selectionManager = *context.selectionManager;
        const auto selectedEntity = selectionManager.GetSelectedEntity();
        if (selectedEntity == entt::null)
        {
            ImGui::Text("No entity selected.");
            return;
        }

        ImGui::Text("Entity selected :)");
    }
}