//
// Created by SkummisDenAndre on 2025-06-16.
//

#include "Engine.h"

#include <chrono>

#include "EventQueue/EngineEvents/EngineEvents.h"
#include "Profiling/Profiling.h"
#include "Renderer/GLFW/GLFWRenderer.h"
#include "Renderer/Null/NullRenderer.h"
#include "ResourceManager/ResourceManager.h"
#include "Systems/SystemContext.h"
#include "Systems/Core/RenderSystem.h"
#include "Utilities/RNGOAsserts.h"
#include "Window/GLFW/GLFWWindow.h"
#include "Window/Null/NullWindow.h"

namespace RNGOEngine::Core
{
    Engine::Engine(const EngineConfig& config)
        : m_running(true),
          m_window(nullptr),
          m_renderer(nullptr),
          m_resourceTracker(),
          m_assetManager(nullptr)
    {
        bool doFlipTexturesVertically = false;

        switch (config.renderType)
        {
            case RenderType::GLFW_OpenGL:
            {
                m_window = std::make_unique<Window::GLFWWindow>(config.width, config.height, config.name);
                m_renderer = std::make_unique<Renderer::GLFWRenderer>();
                doFlipTexturesVertically = true;
                break;
            }
            case RenderType::Headless:
            default:
            {
                m_window = std::make_unique<Window::NullWindow>();
                m_renderer = std::make_unique<Renderer::NullRenderer>();
                break;
            }
        }

        m_resourceManager = std::make_unique<Resources::ResourceManager>(*m_renderer);
        m_assetManager = std::make_unique<AssetHandling::AssetManager>(
            *m_resourceManager, doFlipTexturesVertically);

        for (const auto& [path, type] : config.assetPaths)
        {
            m_assetManager->AddAssetPath(path, type);
        }

        m_rendererAPI = std::make_unique<Renderer::RenderAPI>(
            *m_renderer,
            m_resourceTracker,
            m_assetManager->GetModelManager(),
            m_assetManager->GetShaderManager(),
            m_assetManager->GetMaterialManager(),
            m_assetManager->GetTextureManager(),
            config.width, config.height
        );

        AddEngineSystems();
    }

    void Engine::Run()
    {
        // Initialization

        // Main Loop
        auto lastFrame = std::chrono::high_resolution_clock::now();
        while (m_running)
        {
            RNGO_ZONE_SCOPE;
            RNGO_ZONE_NAME_C("Engine::Run - Main Loop");

            float deltaTime = std::chrono::duration<float>(
                std::chrono::high_resolution_clock::now() - lastFrame).count();
            lastFrame = std::chrono::high_resolution_clock::now();

            // TODO: Make a load pending scene function instead. Keep implementation details out of game loop.
            m_sceneManager.SwitchToPendingScene(*this);
            RNGO_ASSERT(m_sceneManager.GetCurrentScene() && "No scene loaded.");

            PollWindowEvents();

            UpdateEngineSystems(deltaTime);
            UpdateGameSystems(deltaTime);

            Render();

            PollGameEvents();
            PollEngineEvents();
            ClearEvents();
            CheckUnusedResources();

            ++m_frameCount;
            RNGO_FRAME_MARK;
        }

        // Shutdown
        m_resourceManager->DestroyAllResources();
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
            moreEvents = moreEvents || m_rendererAPI->ListenSendEvents(m_eventQueue);
        } while (moreEvents);

        m_inputManager.Update(m_eventQueue);
    }

    void Engine::UpdateEngineSystems(float deltaTime)
    {
        RNGO_ZONE_SCOPE;
        RNGO_ZONE_NAME_C("Engine::UpdateEngineSystems");

        m_engineContext.deltaTime = deltaTime;

        // These don't need to be set every frame. But alas.
        m_engineContext.sceneManager = &m_sceneManager;
        m_engineContext.inputManager = &m_inputManager;
        m_engineContext.jobSystem = &m_jobSystem;
        m_engineContext.assetManager = m_assetManager.get();
        m_engineContext.eventQueue = &m_eventQueue;
        m_engineContext.renderer = m_rendererAPI.get();

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
        m_gameContext.inputManager = &m_inputManager;
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

        m_rendererAPI->Render(*m_window, m_frameCount);
        m_window->SwapBuffers();
    }

    void Engine::PollGameEvents()
    {
        m_inputManager.GetPendingChanges(m_eventQueue);
        m_window->PollGameEvents(m_eventQueue);
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

    void Engine::CheckUnusedResources()
    {
        RNGO_ZONE_SCOPE;
        RNGO_ZONE_NAME_C("Engine::CheckUnusedResources");
        // TODO: Check only every RESOURCE_CHECK_INTERVAL frames.
        const auto unusedResources = m_resourceTracker.GetUnusedResources(m_frameCount, 300);

        if (!unusedResources.IsEmpty())
        {
            m_resourceManager->DestroyMeshes(unusedResources.meshes);
        }
    }

    void Engine::AddEngineSystems()
    {
        m_engineSystems.RegisterSystem<Systems::Core::RenderSystem>();
    }
}