//
// Created by SkummisDenAndre on 2025-06-16.
//

#include "Engine.h"

#include <chrono>

#include "EventQueue/EngineEvents/EngineEvents.h"
#include "Profiling/Profiling.h"
#include "Renderer/GLFW/GLFWRenderer.h"
#include "Renderer/Null/NullRenderer.h"
#include "Systems/SystemContext.h"
#include "Systems/Core/RenderSystem.h"
#include "Window/GLFW/GLFWWindow.h"
#include "Window/Null/NullWindow.h"

namespace RNGOEngine::Core
{
    Engine::Engine(const EngineConfig config)
        : m_running(true),
          m_window(nullptr),
          m_renderer(nullptr),
          m_assetManager(nullptr)
    {
        bool doFlipTexturesVertically = false;

        if (config == EngineConfig::Headless)
        {
            m_window = std::make_unique<Window::NullWindow>();
            m_renderer = std::make_unique<Renderer::NullRenderer>();
        }
        else if (config == EngineConfig::GLFW_OpenGL)
        {
            // TODO: Temporary arbitrary height, width and name.
            m_window = std::make_unique<Window::GLFWWindow>(800, 600, "RNGOEngine");
            m_renderer = std::make_unique<Renderer::GLFWRenderer>(800, 600);
            doFlipTexturesVertically = true;
        }

        m_assetManager = std::make_unique<AssetHandling::AssetManager>(*m_renderer, doFlipTexturesVertically);

        // Add RenderSystem
        AddEngineSystems();
    }

    void Engine::Run()
    {
        auto lastFrame = std::chrono::high_resolution_clock::now();

        while (m_running)
        {
            RNGO_ZONE_SCOPE;
            RNGO_ZONE_NAME_C("Engine::Run - Main Loop");
            
            float deltaTime = std::chrono::duration<float>(
                std::chrono::high_resolution_clock::now() - lastFrame).count();
            lastFrame = std::chrono::high_resolution_clock::now();

            m_sceneManager.SwitchToPendingScene(*this);
            assert(m_sceneManager.GetCurrentScene() && "No scene loaded.");

            PollWindowEvents();

            UpdateEngineSystems(deltaTime);
            UpdateGameSystems(deltaTime);

            Render();

            PollEngineEvents();
            ClearEvents();


            RNGO_FRAME_MARK;
        }
    }

    void Engine::PollWindowEvents()
    {
        m_window->PollWindowEvents(m_eventQueue);
        m_window->PollKeyboardEvents(m_eventQueue);
        m_window->PollMouseEvents(m_eventQueue);

        // Listen to events in a loop to allow for event chaining.
        bool moreEvents;

        do
        {
            moreEvents = m_window->ListenSendEvents(m_eventQueue);
            moreEvents = moreEvents || m_renderer->ListenSendEvents(m_eventQueue);
        } while (moreEvents);
    }

    void Engine::UpdateEngineSystems(float deltaTime)
    {
        RNGO_ZONE_SCOPE;
        RNGO_ZONE_NAME_C("Engine::UpdateEngineSystems");
        
        m_engineContext.deltaTime = deltaTime;

        // These don't need to be set every frame. But alas.
        m_engineContext.sceneManager = &m_sceneManager;
        m_engineContext.jobSystem = &m_jobSystem;
        m_engineContext.assetManager = m_assetManager.get();
        m_engineContext.eventQueue = &m_eventQueue;
        m_engineContext.renderer = m_renderer.get();

        m_engineSystems.Update(*m_sceneManager.GetCurrentWorld(), m_engineContext);
        
        m_engineContext.resourceMapper.ClearTransientResources();
    }

    void Engine::UpdateGameSystems(const float deltaTime)
    {
        RNGO_ZONE_SCOPE;
        RNGO_ZONE_NAME_C("Engine::UpdateGameSystems");
        
        m_gameContext.deltaTime = deltaTime;

        // These don't need to be set every frame. But alas.
        m_gameContext.sceneManager = &m_sceneManager;
        m_gameContext.jobSystem = &m_jobSystem;
        m_gameContext.eventQueue = &m_eventQueue;
        m_gameContext.assetManager = m_assetManager.get();

        m_gameSystems.Update(*m_sceneManager.GetCurrentWorld(), m_gameContext);

        m_gameContext.resourceMapper.ClearTransientResources();
    }

    void Engine::Render() const
    {
        RNGO_ZONE_SCOPE;
        RNGO_ZONE_NAME_C("Engine::Render");
        
        if (m_window->GetHeight() == 0 || m_window->GetWidth() == 0)
        {
            return;
        }

        m_renderer->Render(*m_window);
        m_window->SwapBuffers();
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

    void Engine::AddEngineSystems()
    {
        m_engineSystems.RegisterSystem<Systems::Core::RenderSystem>();
    }
}