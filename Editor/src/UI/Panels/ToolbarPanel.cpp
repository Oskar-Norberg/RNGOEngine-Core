//
// Created by Oskar.Norberg on 2026-01-26.
//

#include "UI/Panels/ToolbarPanel.h"

#include "Editor.h"

namespace RNGOEngine::Editor
{
    void ToolbarPanel::Render(UIContext& context)
    {
        IDockablePanel::Render(context);

        DrawPlayControls(*context.Editor);
        DrawGizmoControls(context.gizmoData);
    }

    void ToolbarPanel::DrawPlayControls(Editor& editor)
    {
        const auto editorPlayState = editor.GetPlayState();

        bool canStop = editorPlayState != EditorPlayState::Stop;
        bool canPause = editorPlayState == EditorPlayState::Play;
        bool canPlay = editorPlayState != EditorPlayState::Play;

        ImGui::BeginDisabled(!canStop);
        if (ImGui::Button("Stop"))
        {
            editor.Stop();
        }
        ImGui::EndDisabled();

        ImGui::SameLine();

        ImGui::BeginDisabled(!canPause);
        if (ImGui::Button("Pause"))
        {
            editor.Pause();
        }
        ImGui::EndDisabled();

        ImGui::SameLine();

        ImGui::BeginDisabled(!canPlay);
        if (ImGui::Button("Play"))
        {
            editor.Play();
        }
        ImGui::EndDisabled();

        ImGui::SameLine();
        ImGui::Text("Currently: %s", magic_enum::enum_name(editorPlayState).data());
    }

    void ToolbarPanel::DrawGizmoControls(Gizmo::GizmoData& gizmoData)
    {
        ImGui::Text("Gizmo controls:");
        DrawOperations(gizmoData);
        ImGui::SameLine();
        ImGui::Text("Space:");
        ImGui::SameLine();
        DrawSpace(gizmoData);
    }

    void ToolbarPanel::DrawOperations(Gizmo::GizmoData& gizmoData)
    {
        auto& currentOperation = gizmoData.CurrentGizmoOperation;
        // TODO: Hardcoded hotkey, make configurable?
        if (ImGui::RadioButton("Translate (T)", currentOperation == Gizmo::TRANSLATE))
        {
            currentOperation = Gizmo::TRANSLATE;
        }
        ImGui::SameLine();
        if (ImGui::RadioButton("Rotate (E)", currentOperation == Gizmo::ROTATE))
        {
            currentOperation = Gizmo::ROTATE;
        }
        ImGui::SameLine();
        if (ImGui::RadioButton("Scale (R)", currentOperation == Gizmo::SCALE))
        {
            currentOperation = Gizmo::SCALE;
        }
    }

    void ToolbarPanel::DrawSpace(Gizmo::GizmoData& gizmoData)
    {
        auto& currentMode = gizmoData.CurrentGizmoMode;
        if (ImGui::RadioButton("Local", currentMode == Gizmo::LOCAL))
        {
            currentMode = Gizmo::LOCAL;
        }
        ImGui::SameLine();
        if (ImGui::RadioButton("World", currentMode == Gizmo::WORLD))
        {
            currentMode = Gizmo::WORLD;
        }
    }
}