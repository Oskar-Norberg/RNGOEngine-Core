//
// Created by Oskar.Norberg on 2025-11-13.
//

#include "UI/Panels/DetailsPanel.h"

#include "UI/Panels/DetailsSubPanel/AssetSubPanel.h"
#include "UI/Panels/DetailsSubPanel/EntitySubPanel.h"

namespace RNGOEngine::Editor
{
    void DetailsPanel::Render(UIContext& context)
    {
        IDockablePanel::Render(context);

        UpdateSubPanelBasedOnSelection(context);

        if (!m_subPanel)
        {
            ImGui::Text("No details to show.");
            return;
        }

        ImGui::Text("%s", m_subPanel->GetSubPanelName().data());
        ImGui::Separator();
        m_subPanel->RenderSubPanel(context);
    }
    void DetailsPanel::UpdateSubPanelBasedOnSelection(UIContext& context)
    {
        const auto currentSelection = context.selectionManager.GetCurrentSelection();

        if (std::holds_alternative<EntitySelection>(currentSelection))
        {
            SwitchToSubPanelIfNotOpen<EntityDetailsSubPanel>(context);
        }
        if (std::holds_alternative<AssetSelection>(currentSelection))
        {
            SwitchToSubPanelIfNotOpen<AssetDetailsSubPanel>(context);
        }
    }
}