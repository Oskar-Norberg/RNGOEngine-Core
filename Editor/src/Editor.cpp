//
// Created by Oskar.Norberg on 2025-11-05.
//

#include "Editor.h"

#include "TestScene.h"
#include "Renderer/API/Passes/ForwardPass.h"

namespace RNGOEngine::Editor
{
    Editor::Editor(const EngineConfig& config)
        : Application(config)
    {
        m_sceneManager.LoadScene<Temporary::TestScene>();

        // Should this really be the editor's responsibility?
        // TODO: Add a RenderPipelineConfiguration to the EngineConfig?
        m_rendererAPI->RegisterPass<Core::Renderer::ForwardPass>(*m_renderer, m_window->GetWidth(),
                                                                 m_window->GetHeight());
    }

    void Editor::OnUpdate(const float deltaTime)
    {
        Application::OnUpdate(deltaTime);
        UpdateEngineSystems(deltaTime);
        UpdateGameSystems(deltaTime);
    }

    void Editor::OnRender()
    {
        Application::OnRender();
        m_rendererAPI->RenderToScreen();
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