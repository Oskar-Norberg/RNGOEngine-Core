//
// Created by Oskar.Norberg on 2025-11-05.
//

#include "Editor.h"

#include "TestScene.h"

namespace RNGOEngine::Editor
{
    Editor::Editor(const EngineConfig& config)
        : Application(config)
    {
        m_sceneManager.LoadScene<Temporary::TestScene>();
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
        m_rendererAPI->Render(*m_window, m_frameCount);
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