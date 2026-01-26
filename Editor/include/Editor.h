//
// Created by Oskar.Norberg on 2025-11-05.
//

#pragma once

#include <Application/Application.h>

#include "ECS/EditorSystemContext.h"
#include "Logging/VectorSink.h"
#include "UI/UIManager.h"

namespace RNGOEngine::Editor
{
    enum class EditorPlayState
    {
        Stop,
        Paused,
        Play,
    };

    class Editor : public Application
    {
    public:
        explicit Editor(const EngineConfig& config);

    public:
        void OnUpdate(float deltaTime) override;
        void OnRender() override;

    private:
        EditorPlayState m_editorPlayState = EditorPlayState::Stop;

    private:
        UIContext m_uiContext;
        UIManager m_UIManager;

    private:
        EditorSystemContext m_editorSystemContext;
        UISelectionManager m_selectionManager;
        Systems::SystemScheduler<EditorSystemContext> m_editorSystems;

    private:
        void UpdateEngineSystems(float deltaTime);
        void UpdateEditorSystems(float deltaTime);
        void UpdateGameSystems(float deltaTime);

    private:
        void SetUpUIContext();
        void SetUpEditorContext();
    };
}