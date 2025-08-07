//
// Created by SkummisDenAndre on 2025-06-16.
//

#include "Engine.h"

#include <thread>
#include <chrono>

#include "Systems/SystemContext.h"

void Engine::Run()
{
    while (m_running)
    {
        constexpr float deltaTime = 1000.0f / 60.0f;

        SystemContext context;
        context.deltaTime = deltaTime;

        m_systems.Update(*this, m_currentScene->world, context.deltaTime);

        std::this_thread::sleep_for(
            std::chrono::milliseconds(static_cast<int>(deltaTime))
        );  // ~60fps
    }
}

void Engine::Quit()
{
    m_running = false;
}
