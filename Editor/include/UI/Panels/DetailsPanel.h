//
// Created by Oskar.Norberg on 2025-11-13.
//

#pragma once
#include "UI/IDockablePanel.h"

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
    };
}