//
// Created by SkummisDenAndre on 2025-06-16.
//

#include "Engine.h"

#include <chrono>

#include "EventQueue/EngineEvents/EngineEvents.h"
#include "Profiling/Profiling.h"
#include "Renderer/GLFW/GLFWRenderer.h"
#include "Systems/Core/RenderSystem.h"
#include "Window/GLFW/GLFWWindow.h"

namespace RNGOEngine::Core
{
    Engine::Engine(EngineConfig config)
    {
        if (config == EngineConfig::Headless)
        {
            // TODO: Add NullRenderer and NullWindow.
            m_window = nullptr;
        }
        else if (config == EngineConfig::GLFW_OpenGL)
        {
            // TODO: Temporary arbitrary height, width and name.
            m_window = std::make_unique<Window::GLFWWindow>(800, 600, "RNGOEngine");
            m_renderer = std::make_unique<Renderer::GLFWRenderer>();
        }

        // Add RenderSystem
        RegisterSystem<Systems::Core::RenderSystem>();
    }

    void Engine::Run()
    {
        // TODO: Shitty solution, one engine = one scene.
        // TODO: pleaes fix
        // TODO: fix please
        assert(m_currentScene && "No scene loaded!");
        m_currentScene->Initialize(*this);

        auto lastFrame = std::chrono::high_resolution_clock::now();

        while (m_running)
        {
            float deltaTime = std::chrono::duration<float>(
                std::chrono::high_resolution_clock::now() - lastFrame
            ).count();
            lastFrame = std::chrono::high_resolution_clock::now();

            PollWindowEvents();

            UpdateSystems(deltaTime);

            Render();

            PollEngineEvents();
            ClearEvents();

            RNGO_FRAME_MARK;
        }

        m_currentScene->Exit(*this);
    }

    void Engine::PollWindowEvents()
    {
        if (m_window)
        {
            m_window->PollWindowEvents(m_eventQueue);
            m_window->PollKeyboardEvents(m_eventQueue);
            m_window->PollMouseEvents(m_eventQueue);
        }
    }

    void Engine::UpdateSystems(float deltaTime)
    {
        m_systems.Update(m_eventQueue, *m_renderer, m_currentScene->world, deltaTime);
    }

    void Engine::Render()
    {
        if (m_renderer)
        {
            m_renderer->Render(*m_window);
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