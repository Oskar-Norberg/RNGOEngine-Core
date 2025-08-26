//
// Created by SkummisDenAndre on 2025-06-16.
//

#pragma once

#include <memory>

#include "Concepts/Concepts.h"
#include "Window/IWindow.h"
#include "Renderer/IRenderer.h"
#include "Systems/ISystem.h"
#include "Systems/SystemScheduler.h"
#include "Scene/Scene.h"

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

        // Renderer Helpers
    public:
        Renderer::MeshHandle CreateMesh(std::span<float> vertices, std::span<unsigned> indices) const
        {
            return m_renderer->CreateMesh(vertices, indices);
        }

        Renderer::ShaderHandle CreateShader(std::string_view vertexSource, std::string_view fragmentSource) const
        {
            return m_renderer->CreateShader(vertexSource, fragmentSource);
        }

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

    private:
        void PollWindowEvents();
        void PollEngineEvents();
        void ClearEvents();
    };
}