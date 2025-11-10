//
// Created by Oskar.Norberg on 2025-11-10.
//

#pragma once

#include <memory>
#include <vector>

#include "IDockablePanel.h"

namespace RNGOEngine
{
    namespace Core
    {
        namespace Window
        {
            class IWindow;
        }
    }
}

namespace RNGOEngine::Editor
{
    // TODO: Trigger panel events (OnOpen, OnClose, OnFocusGained, OnFocusLost, OnResize)
    class UIManager
    {
    public:
        explicit UIManager(Core::Window::IWindow& window);

        void Update(float deltaTime);

        void BeginFrame();
        void Render();
        void EndFrame();

    public:
        template<typename T, typename... Args>
        void RegisterPanel(Args&&... args)
        {
            m_panels.emplace_back(std::make_unique<T>(std::forward<Args>(args)...));
        }

    private:
        std::vector<std::unique_ptr<IDockablePanel>> m_panels;

    private:
        // Reference means this cannot be moved/unbound, but that's probably fine.
        Core::Window::IWindow& m_window;
    };
}