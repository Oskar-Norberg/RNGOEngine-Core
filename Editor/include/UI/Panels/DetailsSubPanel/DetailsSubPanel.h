//
// Created by Oskar.Norberg on 2026-02-09.
//

#pragma once

#include <imgui.h>

#include "UI/UIContext.h"

namespace RNGOEngine::Editor
{
    class DetailsSubPanel
    {
    public:
        virtual ~DetailsSubPanel() = default;
        virtual void RenderSubPanel(UIContext& context) = 0;

    public:
        virtual void OnEnter(UIContext& context)
        {
        }

        virtual void OnExit(UIContext& context)
        {
        }

    public:
        virtual std::string_view GetSubPanelName() const = 0;
    };
}