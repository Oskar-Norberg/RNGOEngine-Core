//
// Created by SkummisDenAndre on 2025-06-16.
//

#include "Engine.h"

#include <chrono>

#include "EventQueue/EngineEvents/EngineEvents.h"
#include "Profiling/Profiling.h"
#include "Window/GLFW/GLFWWindow.h"

namespace RNGOEngine::Core
{
    Engine::Engine(EngineConfig config)
    {
        if (config == EngineConfig::Headless)
        {
            m_window = nullptr;
        }
        else if (config == EngineConfig::GLFW_OpenGL)
        {
            // TODO: Temporary arbitrary height, width and name.
            m_window = std::make_unique<Window::GLFWWindow>(800, 600, "RNGOEngine");
        }
    }

    void Engine::Run()
    {
        auto lastFrame = std::chrono::high_resolution_clock::now();

        while (m_running)
        {
            float deltaTime = std::chrono::duration<float>(std::chrono::high_resolution_clock::now() - lastFrame).count();
            lastFrame = std::chrono::high_resolution_clock::now();

            PollWindowEvents();

            m_systems.Update(m_eventQueue, m_currentScene->world, deltaTime);

            SwapBuffers();

            PollEngineEvents();
            ClearEvents();

            RNGO_FRAME_MARK;
        }
    }

    void Engine::PollWindowEvents()
    {
        if (m_window)
        {
            m_window->PollEvents(m_eventQueue);
        }
    }

    void Engine::SwapBuffers()
    {
        if (m_window)
        {
            m_window->SwapBuffers();
        }
    }

    void Engine::PollEngineEvents()
    {
        if (!m_eventQueue.GetEvents<Events::ExitEvent>().empty())
        {
            m_running = false;
        }
    }

    void Engine::ClearEvents()
    {
        m_eventQueue.Clear();
    }
}