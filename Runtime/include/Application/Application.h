//
// Created by Oskar.Norberg on 2025-11-05.
//

#pragma once

#include <memory>

#include "Assets/AssetDatabase/AssetDatabase.h"
#include "Assets/AssetFetcher/AssetFetcher.h"
#include "Assets/AssetLoader.h"
#include "Assets/RuntimeAssetRegistry/RuntimeAssetRegistry.h"
#include "InputManager/InputManager.h"
#include "Renderer/API/RenderAPI.h"
#include "Renderer/IRenderer.h"
#include "Scene/SceneManager/SceneManager.h"
#include "Systems/SystemContext.h"
#include "Systems/SystemScheduler.h"
#include "Utilities/JobSystem/JobSystem.h"
#include "Window/IWindow.h"

namespace RNGOEngine
{
    enum class RenderType
    {
        Headless,
        GLFW_OpenGL,
    };

    enum class Pipeline
    {
        Forward,
        ForwardPlus,
        Deferred,
    };

    struct EngineConfig
    {
        RenderType RenderType = RenderType::Headless;
        Pipeline Pipeline = Pipeline::Forward;
        size_t Width = 800;
        size_t Height = 600;
        std::string_view Name = "RNGOEngine Application";

        std::span<const std::pair<std::filesystem::path, AssetHandling::AssetType>> AssetPaths;
    };

    class Application
    {
    public:
        explicit Application(const EngineConfig& config);
        virtual ~Application();

    public:
        void Run();

    public:
        virtual void OnUpdate(float deltaTime);
        virtual void OnRender();

    protected:
        bool m_isRunning = true;
        size_t m_frameCount = 0;

        std::unique_ptr<Core::Window::IWindow> m_window;
        std::unique_ptr<Core::Renderer::IRenderer> m_renderer;
        std::unique_ptr<Core::Renderer::RenderAPI> m_rendererAPI;

        // Asset Management
    protected:
        AssetHandling::AssetFetcher m_assetFetcher;
        std::unique_ptr<AssetHandling::AssetLoader> m_assetLoader;
        std::unique_ptr<Resources::ResourceManager> m_resourceManager;
        Resources::ResourceTracker m_resourceTracker;
        AssetHandling::AssetDatabase m_assetDatabase;
        AssetHandling::RuntimeAssetRegistry m_runtimeAssetRegistry;
        std::unique_ptr<AssetHandling::AssetManager> m_assetManager;

        // Threading
    protected:
        Utilities::JobSystem m_jobSystem;

        // Input Management
    protected:
        Core::InputManager m_inputManager;

        // Scene Management
    protected:
        Core::SceneManager m_sceneManager;

        // Systems
    protected:
        Systems::SystemContext m_gameSystemContext;
        Systems::SystemScheduler<Systems::SystemContext> m_gameSystems;
        Systems::EngineSystemContext m_engineSystemContext;
        Systems::SystemScheduler<Systems::EngineSystemContext> m_engineSystems;

        // Event Queue
    protected:
        Events::EventQueue m_eventQueue;

        // Resource Mappers
    protected:
        Systems::Resources::ResourceMapper m_engineResourceMapper;
        Systems::Resources::ResourceMapper m_gameResourceMapper;

        // Internals
    private:
        void PollWindowEvents();
        void PollGameEvents();
        void PollEngineEvents();

    private:
        void ClearEvents();

        // Setup
    private:
        void SetupSystemContexts();
        void AddEngineSystems();
    };
}