//
// Created by Oskar.Norberg on 2025-11-25.
//

#pragma once

#include "UI/IDockablePanel.h"

namespace RNGOEngine::Editor
{
    class ConsolePanel : public IDockablePanel
    {
    public:
        void Render(UIContext& context) override;
        
        std::string_view GetPanelName() const override
        {
            return "Console";
        }
    };
}