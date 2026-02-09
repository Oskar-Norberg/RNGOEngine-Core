//
// Created by Oskar.Norberg on 2025-11-13.
//

#pragma once

#include "DetailsSubPanel/DetailsSubPanel.h"
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
        std::unique_ptr<DetailsSubPanel> m_subPanel{};

        void UpdateSubPanelBasedOnSelection(UIContext& context);

        template<Concepts::DerivedFrom<DetailsSubPanel> TSubPanel>
        void SwitchToSubPanelIfNotOpen(UIContext& context)
        {
            // Exists and is already open
            if (m_subPanel && dynamic_cast<TSubPanel*>(m_subPanel.get()))
            {
                return;
            }

            if (m_subPanel)
            {
                m_subPanel->OnExit(context);
            }

            // TODO: Forward arguments? Bah, fix later.
            m_subPanel = std::make_unique<TSubPanel>();
            m_subPanel->OnEnter(context);
        }
    };
    EDITOR_PANEL(DetailsPanel);
}