//
// Created by Oskar.Norberg on 2025-11-10.
//

#pragma once

#include <memory>
#include <vector>

#include "UI/Panels/IDockablePanel.h"

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
    class UIManager
    {
    public:
        explicit UIManager(Core::Window::IWindow& window);

        void Update(float deltaTime);

        void BeginFrame();
        void Render();
        void EndFrame();

    private:
        std::vector<std::unique_ptr<IDockablePanel>> m_panels;

    private:
        // Reference means this cannot be moved/unbound, but that's probably fine.
        Core::Window::IWindow& m_window;
    };
}