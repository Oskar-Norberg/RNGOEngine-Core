//
// Created by Oskar.Norberg on 2026-01-26.
//

#pragma once
#include "UI/IDockablePanel.h"
#include "UI/PanelInitializer.h"

namespace RNGOEngine::Editor
{
    class ToolbarPanel : public IDockablePanel
    {
    public:
        void Render(UIContext& context) override;

        std::string_view GetPanelName() const override
        {
            return "Play Panel";
        }

    private:
        void DrawPlayControls(Editor& editor);

        void DrawGizmoControls(Gizmo::GizmoData& gizmoData);
        void DrawOperations(Gizmo::GizmoData& gizmoData);
        void DrawSpace(Gizmo::GizmoData& gizmoData);
    };
    EDITOR_PANEL(ToolbarPanel);
}