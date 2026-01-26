//
// Created by ringo on 2025-10-03.
//

#pragma once

#include "Concepts/Concepts.h"
#include "Scene/Scene.h"

namespace RNGOEngine::Core
{
    class SceneManager
    {
    public:
        void ClearScene();

        template<Concepts::DerivedFrom<Scene> T>
        void LoadScene();

        void SwitchToPendingScene();

        Scene* GetCurrentScene() const;
        World* GetCurrentWorld() const;

    private:
        std::unique_ptr<Scene> m_currentScene;

        std::unique_ptr<Scene> m_pendingScene;
    };

#include "SceneManager.tpp"
}