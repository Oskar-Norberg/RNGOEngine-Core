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

        if (!m_subPanelManager.HasOpenSubPanel())
        {
            ImGui::Text("No details to show.");
            return;
        }

        ImGui::Text("%s", m_subPanelManager.GetSubPanelName().data());
        ImGui::Separator();
        m_subPanelManager.RenderCurrentSubPanel(context);
    }
    void DetailsPanel::UpdateSubPanelBasedOnSelection(UIContext& context)
    {
        const auto currentSelection = context.selectionManager.GetCurrentSelection();

        if (std::holds_alternative<EntitySelection>(currentSelection))
        {
            m_subPanelManager.SwitchToSubPanelIfNotOpen<EntityDetailsSubPanel>(context);
        }
        if (std::holds_alternative<AssetSelection>(currentSelection))
        {
            m_subPanelManager.SwitchToSubPanelIfNotOpen<AssetDetailsSubPanel>(context);
        }
    }
}