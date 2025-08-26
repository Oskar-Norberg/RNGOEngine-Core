//
// Created by SkummisDenAndre on 2025-06-16.
//

#pragma once

#include <memory>

#include "Concepts/Concepts.h"
#include "Renderer/IRenderer.h"
#include "Scene/Scene.h"
#include "Systems/ISystem.h"
#include "Systems/SystemScheduler.h"
#include "Window/IWindow.h"

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

        template<Concepts::DerivedFrom<Systems::ISystem> T, typename... Args>
        void RegisterSystem(Args&&... args)
        {
            m_systems.RegisterSystem<T>(std::forward<Args>(args)...);
        }

        void Run();

    private:
        bool m_running = true;

        std::unique_ptr<Window::IWindow> m_window;
        std::unique_ptr<Renderer::IRenderer> m_renderer;
        
        std::unique_ptr<Scene> m_currentScene;
        Systems::SystemScheduler m_systems;

        Events::EventQueue m_eventQueue;

    private:
        void UpdateSystems(float deltaTime);

    private:
        void Render();
        void SwapBuffers();

    private:
        void PollWindowEvents();
        void PollEngineEvents();
        void ClearEvents();
    };
}