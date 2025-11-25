//
// Created by Oskar.Norberg on 2025-11-05.
//

#include "Editor.h"

#include "ECS/Systems/FreeFlyCameraSystem.h"
#include "Logging/VectorSink.h"
#include "TestScene.h"
#include "UI/Panels/ConsolePanel.h"
#include "UI/Panels/DetailsPanel.h"
#include "UI/Panels/HierarchyPanel.h"
#include "UI/Panels/StatsPanel.h"
#include "UI/Panels/ViewportPanel.h"

namespace RNGOEngine::Editor
{
    Editor::Editor(const EngineConfig& config)
        : Application(config),
          m_UIManager(*m_window, m_sceneManager),
          m_vectorSink(std::make_shared<Core::VectorSink>())
    {
        // Attach Editor only VectorSink
        m_logger.AttachSink(m_vectorSink);
        
        m_sceneManager.LoadScene<Temporary::TestScene>();

        // Set up Editor Systems
        m_editorSystems.RegisterSystem<FreeFlyCameraSystem>();

        RNGO_LOG(Core::LogLevel::Info, "Sample Info Text");
        RNGO_LOG(Core::LogLevel::Warning, "Sample Warning Text");
        RNGO_LOG(Core::LogLevel::Error, "Sample Error Text");
        RNGO_LOG(Core::LogLevel::Critical, "Sample Critical Text");
        RNGO_LOG(Core::LogLevel::Debug, "Sample Debug Text");


        // Set up UI Panels
        m_UIManager.RegisterPanel<StatsPanel>();
        m_UIManager.RegisterPanel<ViewPortPanel>(*m_rendererAPI);
        m_UIManager.RegisterPanel<HierarchyPanel>();
        m_UIManager.RegisterPanel<DetailsPanel>();
        m_UIManager.RegisterPanel<ConsolePanel>();

        SetUpUIContext();
        SetUpEditorContext();
    }

    void Editor::OnUpdate(const float deltaTime)
    {
        Application::OnUpdate(deltaTime);
        UpdateEditorSystems(deltaTime);

        m_UIManager.Update(m_uiContext, deltaTime);

        UpdateEngineSystems(deltaTime);
        UpdateGameSystems(deltaTime);
    }

    void Editor::OnRender()
    {
        Application::OnRender();

        m_UIManager.BeginFrame();
        m_UIManager.Render(m_uiContext);
        m_UIManager.EndFrame();

        m_window->SwapBuffers();
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
        m_uiContext.loggerSink = m_vectorSink.get();
        m_uiContext.selectionManager = &m_selectionManager;
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