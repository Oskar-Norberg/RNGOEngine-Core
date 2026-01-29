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

    public:
        void Stop();
        void Pause();
        void Play();

        EditorPlayState GetPlayState() const;

    private:
        EditorPlayState m_editorPlayState = EditorPlayState::Stop;

    private:
        // Scene cached before entering play mode.
        std::unique_ptr<Core::Scene> m_prePlayScene;

    private:
        UIContext m_uiContext;
        UIManager m_UIManager;

    private:
        EditorSystemContext m_editorSystemContext;
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