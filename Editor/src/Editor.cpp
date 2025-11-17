//
// Created by Oskar.Norberg on 2025-11-05.
//

#include "Editor.h"

#include "ECS/Systems/FreeFlyCameraSystem.h"
#include "TestScene.h"
#include "UI/Panels/DetailsPanel.h"
#include "UI/Panels/HierarchyPanel.h"
#include "UI/Panels/StatsPanel.h"
#include "UI/Panels/ViewportPanel.h"

namespace RNGOEngine::Editor
{
    Editor::Editor(const EngineConfig& config)
        : Application(config), m_UIManager(*m_window, m_sceneManager)
    {
        m_sceneManager.LoadScene<Temporary::TestScene>();

        // Set up Editor Systems
        m_editorSystems.RegisterSystem<FreeFlyCameraSystem>();

        // Set up UI Panels
        m_UIManager.RegisterPanel<StatsPanel>();
        m_UIManager.RegisterPanel<ViewPortPanel>(*m_rendererAPI);
        m_UIManager.RegisterPanel<HierarchyPanel>();
        m_UIManager.RegisterPanel<DetailsPanel>();

        SetUpEditorContext();
    }

    void Editor::OnUpdate(const float deltaTime)
    {
        Application::OnUpdate(deltaTime);
        UpdateEditorSystems(deltaTime);

        m_UIManager.Update(deltaTime);

        UpdateEngineSystems(deltaTime);
        UpdateGameSystems(deltaTime);
    }

    void Editor::OnRender()
    {
        Application::OnRender();

        m_UIManager.BeginFrame();
        m_UIManager.Render();
        m_UIManager.EndFrame();

        m_window->SwapBuffers();
    }

    void Editor::UpdateEngineSystems(float deltaTime)
    {
        m_engineSystemContext.deltaTime = deltaTime;
        m_engineSystems.Update(*m_sceneManager.GetCurrentWorld(), m_engineSystemContext);
    }

    void Editor::UpdateEditorSystems(float deltaTime)
    {
        m_editorSystemContext.deltaTime = deltaTime;
        m_editorSystems.Update(*m_sceneManager.GetCurrentWorld(), m_editorSystemContext);
    }

    void Editor::UpdateGameSystems(float deltaTime)
    {
        m_gameSystemContext.deltaTime = deltaTime;
        m_gameSystems.Update(*m_sceneManager.GetCurrentWorld(), m_gameSystemContext);
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