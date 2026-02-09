//
// Created by Oskar.Norberg on 2025-11-13.
//

#include "UI/Panels/DetailsPanel.h"

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
    void DetailsPanel::UpdateSubPanelBasedOnSelection(const UIContext& context)
    {
        if (context.selectionManager.GetSelectedEntity() != entt::null)
        {
            if (!m_subPanel || dynamic_cast<EntityDetailsSubPanel*>(m_subPanel.get()) == nullptr)
            {
                m_subPanel = std::make_unique<EntityDetailsSubPanel>();
            }
        }
        else
        {
            m_subPanel.reset();
        }
    }
}