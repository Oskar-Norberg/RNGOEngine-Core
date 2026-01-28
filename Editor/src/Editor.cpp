//
// Created by Oskar.Norberg on 2025-11-05.
//

#include "Editor.h"

#include "ECS/Systems/FreeFlyCameraSystem.h"
#include "UI/Panels/DetailsPanel.h"
#include "UI/Panels/ViewportPanel.h"

namespace RNGOEngine::Editor
{
    Editor::Editor(const EngineConfig& config)
        : Application(config), m_UIManager(*m_window)
    {

        // Load Empty
        m_sceneManager.LoadScene<Core::Scene>();

        // Set up Editor Systems
        m_editorSystems.RegisterSystem<FreeFlyCameraSystem>();

        SetUpUIContext();
        SetUpEditorContext();
    }

    void Editor::OnUpdate(const float deltaTime)
    {
        RNGO_ZONE_SCOPED_N("Editor::OnUpdate");

        Application::OnUpdate(deltaTime);
        UpdateEditorSystems(deltaTime);

        m_UIManager.Update(m_uiContext, deltaTime);

        // TODO: Should Engine systems run when paused?
        UpdateEngineSystems(deltaTime);
        if (m_editorPlayState == EditorPlayState::Play)
        {
            UpdateGameSystems(deltaTime);
        }
    }

    void Editor::OnRender()
    {
        RNGO_ZONE_SCOPED_N("Editor::OnRender");

        Application::OnRender();

        m_UIManager.BeginFrame();
        m_UIManager.Render(m_uiContext);
        m_UIManager.EndFrame();
    }

    void Editor::UpdateEngineSystems(float deltaTime)
    {
        m_engineSystemContext.DeltaTime = deltaTime;
        m_engineSystems.Update(*m_sceneManager.GetCurrentWorld(), m_engineSystemContext);
    }

    void Editor::UpdateEditorSystems(float deltaTime)
    {
        m_editorSystemContext.deltaTime = deltaTime;
        m_editorSystems.Update(*m_sceneManager.GetCurrentWorld(), m_editorSystemContext);
    }

    void Editor::UpdateGameSystems(float deltaTime)
    {
        m_gameSystemContext.DeltaTime = deltaTime;
        m_gameSystems.Update(*m_sceneManager.GetCurrentWorld(), m_gameSystemContext);
    }

    void Editor::SetUpUIContext()
    {
        // Set up UIContext
        m_uiContext.editorPlayState = &m_editorPlayState;
        m_uiContext.loggerSink = m_vectorSink.value().get();
        m_uiContext.sceneManager = &m_sceneManager;
        m_uiContext.rendererAPI = m_rendererAPI.get();
    }

    void Editor::SetUpEditorContext()
    {
        m_editorSystemContext.engineResourceMapper = &m_engineResourceMapper;
        m_editorSystemContext.gameResourceMapper = &m_gameResourceMapper;
        m_editorSystemContext.sceneManager = &m_sceneManager;
        m_editorSystemContext.inputManager = &m_inputManager;
        m_editorSystemContext.jobSystem = &m_jobSystem;
        m_editorSystemContext.eventQueue = &m_eventQueue;
        m_editorSystemContext.assetManager = m_assetManager.get();
        m_editorSystemContext.renderer = m_rendererAPI.get();
    }
}