//
// Created by SkummisDenAndre on 2025-06-16.
//

#pragma once

#include <memory>

#include "AssetManager/AssetManager.h"
#include "ResourceManager/ResourceManager.h"
#include "Concepts/Concepts.h"
#include "Window/IWindow.h"
#include "Renderer/IRenderer.h"
#include "Renderer/API/RenderAPI.h"
#include "ResourceManager/ResourceTracker.h"
#include "Systems/ISystem.h"
#include "Systems/SystemScheduler.h"
#include "Scene/Scene.h"
#include "Scene/SceneManager/SceneManager.h"
#include "Systems/SystemContext.h"
#include "Utilities/JobSystem/JobSystem.h"

namespace RNGOEngine::Core
{
    enum class RenderType
    {
        Headless,
        GLFW_OpenGL,
    };

    struct EngineConfig
    {
        RenderType renderType = RenderType::Headless;
        size_t width = 800;
        size_t height = 600;
        std::string name = "RNGOEngine";

        // TODO: No reason for this to be a vector, make it a span instead.
        std::vector<std::pair<std::filesystem::path, AssetHandling::AssetPathType>> assetPaths;
    };

    class Engine
    {
    public:
        explicit Engine(const EngineConfig& config);

        template<Concepts::DerivedFrom<Scene> T>
        void LoadScene()
        {
            m_sceneManager.LoadScene<T>();
        }

        template<Concepts::DerivedFrom<Systems::IGameSystem> T, typename... Args>
        void RegisterSystem(Args&&... args)
        {
            m_gameSystems.RegisterSystem<T>(std::forward<Args>(args)...);
        }

        void Run();

    public:
        AssetHandling::AssetManager& GetAssetManager() const
        {
            return *m_assetManager;
        }

    private:
        // TODO: Break into EngineSettings data-only header?
        // constexpr static size_t RESOURCE_CHECK_INTERVAL = 300; // Frames
        constexpr static size_t RESOURCE_UNUSED_THRESHOLD = 5; // Frames

    private:
        bool m_running;
        // TODO: Consider adding frame count to SystemContext, or wrapping it in a TimeContext.
        size_t m_frameCount = 0;

        std::unique_ptr<Window::IWindow> m_window;
        std::unique_ptr<Renderer::IRenderer> m_renderer;
        std::unique_ptr<Renderer::RenderAPI> m_rendererAPI;
        
        std::unique_ptr<Resources::ResourceManager> m_resourceManager;
        Resources::ResourceTracker m_resourceTracker;
        std::unique_ptr<AssetHandling::AssetManager> m_assetManager;
        Utilities::JobSystem m_jobSystem;
        SceneManager m_sceneManager;
        InputManager m_inputManager;
        
        Systems::SystemContext m_gameContext;
        Systems::SystemScheduler<Systems::SystemContext> m_gameSystems;

        Systems::EngineSystemContext m_engineContext;
        Systems::SystemScheduler<Systems::EngineSystemContext> m_engineSystems;

        Events::EventQueue m_eventQueue;

    private:
        void UpdateEngineSystems(float deltaTime);
        void UpdateGameSystems(float deltaTime);

    private:
        void Render() const;

    private:
        void PollWindowEvents();
        void PollGameEvents();
        void PollEngineEvents();
        void ClearEvents();
        void CheckUnusedResources();

    private:
        void AddEngineSystems();
    };
}