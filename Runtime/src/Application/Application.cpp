//
// Created by Oskar.Norberg on 2025-11-05.
//

#include "Application/Application.h"

#include "Assets/AssetManager/AssetManager.h"
#include "Assets/Bootstrapper/AssetImporterBootstrapper.h"
#include "Assets/Builtin/BuiltinAssetBootstrapper.h"
#include "Renderer/API/Passes/DirectionalShadowMapPass.h"
#include "Renderer/API/Passes/ForwardPass.h"
#include "Renderer/API/Passes/ForwardScreenPass.h"
#include "Renderer/Null/NullRenderer.h"
#include "Renderer/OpenGL/OpenGLRenderer.h"
#include "Systems/Core/Physics/CollisionSystem.h"
#include "Systems/Core/Physics/RigidbodyGravitySystem.h"
#include "Systems/Core/Rendering/BeginFrameSystem.h"
#include "Systems/Core/Rendering/Debug/RenderDebugCollidersSystem.h"
#include "Systems/Core/Rendering/Debug/RenderDebugCollisionsSystem.h"
#include "Systems/Core/Rendering/EndFrameSystem.h"
#include "Systems/Core/Rendering/RenderSystem.h"
#include "Window/GLFW/GLFWWindow.h"
#include "Window/Null/NullWindow.h"

namespace RNGOEngine
{
    Application::Application(const EngineConfig& config)
    {
        if (config.LogToVectorSink)
        {
            m_vectorSink = std::make_shared<Core::VectorSink>();
            m_logger.AttachSink(m_vectorSink.value());
            m_vectorSink->get()->set_level(spdlog::level::debug);
        }

#ifdef RNGOENGINE_TRACY_ENABLE
        RNGO_LOG(Core::LogLevel::Info, "Profiling with Tracy enabled.");
#endif

        bool doFlipTexturesVertically = false;

        // Set Up Renderer
        switch (config.Renderer)
        {
            case RenderType::GLFW_OpenGL:
            {
                m_window =
                    std::make_unique<Core::Window::GLFWWindow>(config.Width, config.Height, config.Name);
                m_renderer = std::make_unique<Core::Renderer::OpenGLRenderer>();
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
        for (const auto& path : config.AssetPaths)
        {
            m_assetFetcher.AddAssetPath(path);
        }

        // Resource Manager
        m_resourceManager = std::make_unique<Resources::ResourceManager>(*m_renderer);

        // Asset Loader and Importers
        m_assetLoader = std::make_unique<AssetHandling::AssetLoader>(
            m_runtimeAssetRegistry, m_assetDatabase, m_assetFetcher
        );
        AssetHandling::BootstrapContext context = {*m_assetLoader, doFlipTexturesVertically};
        AssetHandling::AssetImporterBootstrapper::Bootstrap(context);

        m_assetFetcher.ForEachOfExtension(
            ".meta",
            [this](const std::filesystem::path& metaPath)
            {
                m_assetLoader->Register(metaPath);
            }
        );

        // Asset Managers
        m_assetManager = std::make_unique<AssetHandling::AssetManager>(*m_resourceManager);

        // Set up RenderAPI
        m_rendererAPI = std::make_unique<Core::Renderer::RenderAPI>(*m_renderer, config.Width, config.Height);
        switch (config.Pipeline)
        {
            case PipelineType::Forward:
            {
                m_rendererAPI->RegisterPass<Core::Renderer::DirectionalShadowMapPass>(
                    *m_renderer, m_window->GetWidth(), m_window->GetHeight()
                );
                m_rendererAPI->RegisterPass<Core::Renderer::ForwardPass>(
                    *m_renderer, m_window->GetWidth(), m_window->GetHeight()
                );
                m_rendererAPI->RegisterPass<Core::Renderer::ForwardScreenPass>(
                    *m_renderer, m_window->GetWidth(), m_window->GetHeight()
                );
            }
            break;
            case PipelineType::ForwardPlus:
                RNGO_ASSERT(false && "Forward Plus pipeline not implemented.");
                break;
            case PipelineType::Deferred:
                RNGO_ASSERT(false && "Deferred pipeline not implemented.");
                break;
        }

        // Builtin Assets
        AssetHandling::BuiltinAssets::InitializeBuiltinAssets();
        // TODO: Ugly
        m_assetLoader->LoadPendingAssets(Data::ThreadType::Render);

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
            RNGO_ZONE_SCOPED_N("Application::Run");

            const float deltaTime =
                std::chrono::duration<float>(std::chrono::high_resolution_clock::now() - lastFrame).count();
            lastFrame = std::chrono::high_resolution_clock::now();

            SwitchPendingScene();

            PollWindowEvents();

            OnUpdate(deltaTime);
            OnRender();

            SwapBuffers();

            PollGameEvents();
            PollEngineEvents();
            ClearEvents();

            ++m_frameCount;

            RNGO_FRAME_MARK;
        }
    }

    void Application::OnUpdate(float deltaTime)
    {
    }

    void Application::OnRender()
    {
        m_assetLoader->LoadPendingAssets(Data::ThreadType::Render);
    }

    void Application::SwitchPendingScene()
    {
        RNGO_ZONE_SCOPED_N("Application::SwitchPendingScene");

        m_sceneManager.SwitchToPendingScene();
        RNGO_ASSERT(m_sceneManager.GetCurrentScene() && "No scene loaded.");
    }

    void Application::SwapBuffers()
    {
        RNGO_ZONE_SCOPED_N("Application::SwapBuffers");

        m_window->SwapBuffers();
    }

    void Application::PollWindowEvents()
    {
        RNGO_ZONE_SCOPED_N("Application::PollWindowEvents");

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
        RNGO_ZONE_SCOPED_N("Application::PollGameEvents");

        m_inputManager.GetPendingChanges(m_eventQueue);
        m_window->PollGameEvents(m_eventQueue);
    }

    void Application::PollEngineEvents()
    {
        RNGO_ZONE_SCOPED_N("Application::PollEngineEvents");

        if (!m_eventQueue.GetEvents<Events::ExitEvent>().empty())
        {
            m_isRunning = false;
        }
    }

    void Application::ClearEvents()
    {
        RNGO_ZONE_SCOPED_N("Application::ClearEvents");

        m_eventQueue.Clear();
    }

    void Application::SetupSystemContexts()
    {
        // Engine System Context
        auto& engineContext = m_engineSystemContext;
        engineContext.EngineResourceMapper = &m_engineResourceMapper;
        engineContext.GameResourceMapper = &m_gameResourceMapper;
        engineContext.SceneManager = &m_sceneManager;
        engineContext.InputManager = &m_inputManager;
        engineContext.JobSystem = &m_jobSystem;
        engineContext.EventQueue = &m_eventQueue;
        engineContext.AssetManager = m_assetManager.get();
        engineContext.Renderer = m_rendererAPI.get();

        // Game System Context
        auto& gameContext = m_gameSystemContext;
        gameContext.ResourceMapper = &m_gameResourceMapper;
        gameContext.SceneManager = &m_sceneManager;
        gameContext.InputManager = &m_inputManager;
        gameContext.JobSystem = &m_jobSystem;
        gameContext.EventQueue = &m_eventQueue;
        engineContext.AssetManager = m_assetManager.get();
    }

    void Application::AddEngineSystems()
    {
        m_engineSystems.RegisterSystem<Systems::Core::CollisionSystem>();
        m_engineSystems.RegisterSystem<Systems::Core::RigidbodyGravitySystem>();

        m_engineSystems.RegisterSystem<Systems::Core::BeginFrameSystem>();
        m_engineSystems.RegisterSystem<Systems::Core::RenderSystem>();
        m_engineSystems.RegisterSystem<Systems::Core::RenderDebugCollidersSystem>();
        m_engineSystems.RegisterSystem<Systems::Core::RenderDebugCollisionsSystem>();
        m_engineSystems.RegisterSystem<Systems::Core::EndFrameSystem>();
    }
}