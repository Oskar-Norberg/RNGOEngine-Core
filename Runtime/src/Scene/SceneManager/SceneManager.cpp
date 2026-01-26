//
// Created by ringo on 2025-10-03.
//

#include "Scene/SceneManager/SceneManager.h"

namespace RNGOEngine::Core
{
    void SceneManager::ClearScene()
    {
        if (m_currentScene)
        {
            m_currentScene->Exit();
            m_currentScene->world.GetRegistry().clear();
            m_currentScene->Initialize();
        }
    }

    void SceneManager::SwitchToPendingScene()
    {
        if (!m_pendingScene)
        {
            return;
        }

        if (m_currentScene)
        {
            m_currentScene->Exit();
        }
        m_currentScene = std::move(m_pendingScene);
        m_currentScene->Initialize();
    }

    Scene* SceneManager::GetCurrentScene() const
    {
        return m_currentScene.get();
    }

    World* SceneManager::GetCurrentWorld() const
    {
        return m_currentScene ? &m_currentScene->world : nullptr;
    }
}