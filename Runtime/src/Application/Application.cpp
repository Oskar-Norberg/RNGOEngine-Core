//
// Created by Oskar.Norberg on 2025-11-05.
//

#include "Application/Application.h"

#include "Assets/AssetManager/AssetManager.h"
#include "Assets/Bootstrapper/AssetImporterBootstrapper.h"
#include "Renderer/API/Passes/ForwardPass.h"
#include "Renderer/API/Passes/ForwardScreenPass.h"
#include "Renderer/GLFW/GLFWRenderer.h"
#include "Renderer/Null/NullRenderer.h"
#include "Systems/Core/RenderSystem.h"
#include "Window/GLFW/GLFWWindow.h"
#include "Window/Null/NullWindow.h"

namespace RNGOEngine
{
    Application::Application(const EngineConfig& config)
    {
        bool doFlipTexturesVertically = false;

        // Set Up Renderer
        switch (config.renderType)
        {
            case RenderType::GLFW_OpenGL:
            {
                m_window =
                    std::make_unique<Core::Window::GLFWWindow>(config.width, config.height, config.name);
                m_renderer = std::make_unique<Core::Renderer::GLFWRenderer>();
                doFlipTexturesVertically = true;
                break;
            }
            case RenderType::Headless:
            default:
            {
                m_window = std::make_unique<Core::Window::NullWindow>();
                m_renderer = std::make_unique<Core::Renderer::NullRenderer>();
                break;
            }
        }

        // Add Asset Paths
        for (const auto& [path, type] : config.assetPaths)
        {
            m_assetFetcher.AddAssetPath(type, path);
        }

        // Resource Manager
        m_resourceManager = std::make_unique<Resources::ResourceManager>(*m_renderer);

        // Asset Loader and Importers
        m_assetLoader = std::make_unique<AssetHandling::AssetLoader>(m_assetDatabase, m_assetFetcher);
        AssetHandling::BootstrapContext context = {*m_assetLoader, doFlipTexturesVertically};
        AssetHandling::AssetImporterBootstrapper::Bootstrap(context);

        // Asset Managers
        m_assetManager = std::make_unique<AssetHandling::AssetManager>(
            m_assetFetcher, m_assetDatabase, *m_resourceManager, doFlipTexturesVertically
        );

        // Set up RenderAPI
        m_rendererAPI = std::make_unique<Core::Renderer::RenderAPI>(*m_renderer, config.width, config.height);
        switch (config.pipeline)
        {
            case Pipeline::Forward:
            {
                m_rendererAPI->RegisterPass<Core::Renderer::ForwardPass>(
                    *m_renderer, m_window->GetWidth(), m_window->GetHeight()
                );
                m_rendererAPI->RegisterPass<Core::Renderer::ForwardScreenPass>(
                    *m_renderer, m_window->GetWidth(), m_window->GetHeight()
                );
            }
            break;
            case Pipeline::ForwardPlus:
                RNGO_ASSERT(false && "Forward Plus pipeline not implemented.");
                break;
            case Pipeline::Deferred:
                RNGO_ASSERT(false && "Deferred pipeline not implemented.");
                break;
        }

        // TODO: TEMPORARY
        m_assetManager->SetShaderImporter(
            m_assetLoader->GetImporter<AssetHandling::ShaderAssetImporter>(AssetHandling::AssetType::Shader)
        );

        AddEngineSystems();
        SetupSystemContexts();
    }

    Application::~Application()
    {
        // TODO: Cleanup x3
    }

    void Application::Run()
    {
        // Main Loop
        // TODO: Use a fixed timestep for system updates.
        // TODO: I don't think high_resolution_clock is guaranteed to never go backwards.
        auto lastFrame = std::chrono::high_resolution_clock::now();
        while (m_isRunning)
        {
            RNGO_ZONE_SCOPE;
            RNGO_ZONE_NAME_C("Engine::Run - Main Loop");

            const float deltaTime =
                std::chrono::duration<float>(std::chrono::high_resolution_clock::now() - lastFrame).count();
            lastFrame = std::chrono::high_resolution_clock::now();

            m_sceneManager.SwitchToPendingScene();
            RNGO_ASSERT(m_sceneManager.GetCurrentScene() && "No scene loaded.");

            PollWindowEvents();

            OnUpdate(deltaTime);
            OnRender();

            PollGameEvents();
            PollEngineEvents();
            ClearEvents();

            ++m_frameCount;
        }
    }

    void Application::OnUpdate(float deltaTime)
    {
    }

    void Application::OnRender()
    {
    }

    void Application::PollWindowEvents()
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

    // TODO: Very hyperspecific function. Just have a pre-update poll and post-update poll
    void Application::PollGameEvents()
    {
        m_inputManager.GetPendingChanges(m_eventQueue);
        m_window->PollGameEvents(m_eventQueue);
    }

    void Application::PollEngineEvents()
    {
        if (!m_eventQueue.GetEvents<Events::ExitEvent>().empty())
        {
            m_isRunning = false;
        }
    }

    void Application::ClearEvents()
    {
        m_eventQueue.Clear();
    }

    void Application::SetupSystemContexts()
    {
        // Engine System Context
        auto& engineContext = m_engineSystemContext;
        engineContext.engineResourceMapper = &m_engineResourceMapper;
        engineContext.gameResourceMapper = &m_gameResourceMapper;
        engineContext.sceneManager = &m_sceneManager;
        engineContext.inputManager = &m_inputManager;
        engineContext.jobSystem = &m_jobSystem;
        engineContext.eventQueue = &m_eventQueue;
        engineContext.assetManager = m_assetManager.get();
        engineContext.renderer = m_rendererAPI.get();

        // Game System Context
        auto& gameContext = m_gameSystemContext;
        gameContext.resourceMapper = &m_gameResourceMapper;
        gameContext.sceneManager = &m_sceneManager;
        gameContext.inputManager = &m_inputManager;
        gameContext.jobSystem = &m_jobSystem;
        gameContext.eventQueue = &m_eventQueue;
        engineContext.assetManager = m_assetManager.get();
    }

    void Application::AddEngineSystems()
    {
        m_engineSystems.RegisterSystem<Systems::Core::RenderSystem>();
    }
}