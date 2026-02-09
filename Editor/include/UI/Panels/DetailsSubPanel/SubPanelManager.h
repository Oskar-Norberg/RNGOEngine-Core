//
// Created by Oskar.Norberg on 2026-02-09.
//

#pragma once

#include "DetailsSubPanel.h"

namespace RNGOEngine::Editor
{
    class SubPanelManager
    {
    public:
        /**
         *
         * @param context UIContext to pass to subpanels draw function.
         * @return True if a subpanel was open and rendered, false otherwise.
         */
        bool RenderCurrentSubPanel(UIContext& context) const
        {
            if (m_subPanel)
            {
                m_subPanel->RenderSubPanel(context);
                return true;
            }

            return false;
        }

        bool HasOpenSubPanel() const
        {
            return m_subPanel != nullptr;
        }

        DetailsSubPanel* GetSubPanel() const
        {
            return m_subPanel.get();
        }

        std::string_view GetSubPanelName() const
        {
            return m_subPanel ? m_subPanel->GetSubPanelName() : "No SubPanel";
        }

    public:
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

    private:
        std::unique_ptr<DetailsSubPanel> m_subPanel{};
    };
}