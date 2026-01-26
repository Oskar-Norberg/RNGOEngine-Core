//
// Created by Oskar.Norberg on 2026-01-26.
//

#pragma once
#include "UI/IDockablePanel.h"

namespace RNGOEngine::Editor
{
    class PlayPanel : public IDockablePanel
    {
    public:
        void Render(UIContext& context) override;

        std::string_view GetPanelName() const override
        {
            return "Play Panel";
        }
    };
}