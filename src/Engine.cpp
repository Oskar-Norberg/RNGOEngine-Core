//
// Created by SkummisDenAndre on 2025-06-16.
//

#include "Engine.h"

#include <thread>
#include <chrono>

void Engine::Run()
{
    while (m_running)
    {
        constexpr float deltaTime = 1000.0f / 60.0f;

        m_systems.Update(*this, m_currentScene->world, deltaTime);

        std::this_thread::sleep_for(
            std::chrono::milliseconds(static_cast<int>(deltaTime))
        );  // ~60fps
    }
}

void Engine::Quit()
{
    m_running = false;
}
