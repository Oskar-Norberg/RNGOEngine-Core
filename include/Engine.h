//
// Created by SkummisDenAndre on 2025-06-16.
//

#pragma once

#include <memory>

#include "AssetManager/AssetManager.h"
#include "Concepts/Concepts.h"
#include "Window/IWindow.h"
#include "Renderer/IRenderer.h"
#include "Systems/ISystem.h"
#include "Systems/SystemScheduler.h"
#include "Scene/Scene.h"
#include "Systems/SystemContext.h"
#include "Utilities/JobSystem/JobSystem.h"

namespace RNGOEngine::Core
{
    enum EngineConfig
    {
        Headless,
        GLFW_OpenGL,
    };

    class Engine
    {
    public:
        explicit Engine(EngineConfig config);

        template<Concepts::DerivedFrom<Scene> T>
        void LoadScene()
        {
            m_currentScene = std::make_unique<T>();
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
        bool m_running;

        std::unique_ptr<Window::IWindow> m_window;
        std::unique_ptr<Renderer::IRenderer> m_renderer;
        std::unique_ptr<AssetHandling::AssetManager> m_assetManager;
        Utilities::JobSystem m_jobSystem;

        std::unique_ptr<Scene> m_currentScene;

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
        void PollEngineEvents();
        void ClearEvents();

    private:
        void AddEngineSystems();
    };
}