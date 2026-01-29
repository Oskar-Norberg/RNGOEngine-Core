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
            m_currentScene->world.GetRegistry().clear();
        }
    }

    void SceneManager::SwitchToPendingScene()
    {
        if (!m_pendingScene)
        {
            return;
        }

        m_currentScene = std::move(m_pendingScene);
    }

    Scene* SceneManager::GetCurrentScene() const
    {
        return m_currentScene.get();
    }

    World* SceneManager::GetCurrentWorld() const
    {
        return m_currentScene ? &m_currentScene->world : nullptr;
    }

    const std::unique_ptr<Scene>& SceneManager::GetScene() const
    {
        return m_currentScene;
    }

    void SceneManager::LoadScene(std::unique_ptr<Scene>&& scene)
    {
        m_currentScene = std::move(scene);
    }
}