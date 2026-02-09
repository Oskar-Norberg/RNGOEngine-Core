//
// Created by Oskar.Norberg on 2026-02-09.
//

#pragma once

#include "UI/Panels/DetailsSubPanel/DetailsSubPanel.h"

namespace RNGOEngine::Editor
{
    class MaterialEditor : public DetailsSubPanel
    {
    public:
        void RenderSubPanel(UIContext& context) override
        {
            ImGui::Text("idk, do some material stuffs");
        }

        std::string_view GetSubPanelName() const override
        {
            return "Material Editor";
        }
    };
}