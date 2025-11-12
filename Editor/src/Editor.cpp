//
// Created by Oskar.Norberg on 2025-11-05.
//

#include "Editor.h"

#include "TestScene.h"
#include "Renderer/API/Passes/ForwardPass.h"
#include "Renderer/API/Passes/ForwardScreenPass.h"
#include "UI/Panels/HierarchyPanel.h"
#include "UI/Panels/StatsPanel.h"
#include "UI/Panels/ViewportPanel.h"

namespace RNGOEngine::Editor
{
    Editor::Editor(const EngineConfig& config)
        : Application(config), m_UIManager(*m_window)
    {
        m_sceneManager.LoadScene<Temporary::TestScene>();

        // Should this really be the editor's responsibility?
        // TODO: Add a RenderPipelineConfiguration to the EngineConfig?
        m_rendererAPI->RegisterPass<Core::Renderer::ForwardPass>(*m_renderer, m_window->GetWidth(),
                                                                 m_window->GetHeight());
        m_rendererAPI->RegisterPass<Core::Renderer::ForwardScreenPass>(*m_renderer, m_window->GetWidth(),
                                                                       m_window->GetHeight());

        // Set up UI Panels
        m_UIManager.RegisterPanel<StatsPanel>();
        m_UIManager.RegisterPanel<ViewPortPanel>(*m_rendererAPI);
        m_UIManager.RegisterPanel<HierarchyPanel>(m_sceneManager);
    }

    void Editor::OnUpdate(const float deltaTime)
    {
        Application::OnUpdate(deltaTime);

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

    void Editor::UpdateGameSystems(float deltaTime)
    {
        m_gameSystemContext.deltaTime = deltaTime;
        m_gameSystems.Update(*m_sceneManager.GetCurrentWorld(), m_gameSystemContext);
    }
}