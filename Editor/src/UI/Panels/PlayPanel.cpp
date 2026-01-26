//
// Created by Oskar.Norberg on 2026-01-26.
//

#include "UI/Panels/PlayPanel.h"

#include "Editor.h"

namespace RNGOEngine::Editor
{
    void PlayPanel::Render(UIContext& context)
    {
        IDockablePanel::Render(context);

        auto& editorPlayState = *context.editorPlayState;

        bool canStop = editorPlayState != EditorPlayState::Stop;
        bool canPause = editorPlayState == EditorPlayState::Play;
        bool canPlay = editorPlayState != EditorPlayState::Play;

        ImGui::BeginDisabled(!canStop);
        if (ImGui::Button("Stop"))
        {
            editorPlayState = EditorPlayState::Stop;
        }
        ImGui::EndDisabled();

        ImGui::SameLine();

        ImGui::BeginDisabled(!canPause);
        if (ImGui::Button("Pause"))
        {
            editorPlayState = EditorPlayState::Paused;
        }
        ImGui::EndDisabled();

        ImGui::SameLine();

        ImGui::BeginDisabled(!canPlay);
        if (ImGui::Button("Play"))
        {
            editorPlayState = EditorPlayState::Play;
        }
        ImGui::EndDisabled();

        ImGui::SameLine();
        ImGui::Text("Currently: %s", magic_enum::enum_name(editorPlayState).data());
    }
}