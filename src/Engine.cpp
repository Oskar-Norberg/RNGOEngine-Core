//
// Created by SkummisDenAndre on 2025-06-16.
//

#include "Engine.h"

#include <chrono>
#include "Profiling/Profiling.h"


void Engine::Run()
{
    auto lastFrame = std::chrono::high_resolution_clock::now();

    while (m_running)
    {
        RNGO_FRAME_START("Engine::Run");

        float deltaTime = std::chrono::duration<float>(std::chrono::high_resolution_clock::now() - lastFrame).count();
        lastFrame = std::chrono::high_resolution_clock::now();

        m_systems.Update(*this, m_currentScene->world, deltaTime);

        RNGO_FRAME_END("Engine::Run");
    }
}

void Engine::Quit()
{
    m_running = false;
}
