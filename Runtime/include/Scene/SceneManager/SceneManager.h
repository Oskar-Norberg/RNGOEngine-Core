//
// Created by ringo on 2025-10-03.
//

#pragma once

#include "Utilities/Concepts/Concepts.h"
#include "Scene/Scene.h"

namespace RNGOEngine::Core
{
    // TODO: Why isn't this doing the loading from file?
    class SceneManager
    {
    public:
        void ClearScene();

        template<Concepts::DerivedFrom<Scene> T>
        void LoadScene();

        void SwitchToPendingScene();

        Scene* GetCurrentScene() const;
        World* GetCurrentWorld() const;

    public:
        const std::unique_ptr<Scene>& GetScene() const;
        void LoadScene(std::unique_ptr<Scene>&& scene);

    private:
        std::unique_ptr<Scene> m_currentScene;

        std::unique_ptr<Scene> m_pendingScene;
    };

#include "SceneManager.inl"
}