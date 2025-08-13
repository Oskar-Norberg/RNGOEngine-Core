//
// Created by SkummisDenAndre on 2025-06-16.
//

#pragma once

#include <memory>

#include "Concepts/Concepts.h"
#include "Scene/Scene.h"
#include "Systems/ISystem.h"
#include "Systems/SystemScheduler.h"

namespace RNGOEngine::Core
{
    class Engine
    {
    public:
        template<Concepts::DerivedFrom<Scene> T>
        void LoadScene()
        {
            m_currentScene = std::make_unique<T>();
        }

        template<Concepts::DerivedFrom<Systems::ISystem> T, typename... Args>
        void RegisterSystem(Args&&... args)
        {
            m_systems.RegisterSystem<T>(std::forward<Args>(args)...);
        }

        void Run();

        void Quit();

    private:
        bool m_running = true;
        // I hate that these are heap allocated.
        std::unique_ptr<Scene> m_currentScene;
        Systems::SystemScheduler m_systems;
    };
}