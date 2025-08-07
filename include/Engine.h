//
// Created by SkummisDenAndre on 2025-06-16.
//

#pragma once

#include "Concepts/Concepts.h"
#include "Scene/Scene.h"
#include "Systems/SystemScheduler.h"

class Engine
{
public:
    template<DerivedFrom<Scene> T>
    void LoadScene()
    {
        m_currentScene = std::make_unique<T>();
    }

    template<DerivedFrom<ISystem> T, typename... Args>
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
    SystemScheduler m_systems;
};
