//
// Created by Oskar.Norberg on 2025-11-10.
//

#pragma once


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

        void BeginFrame();
        void Render();
        void EndFrame();

        // Reference means this cannot be moved/unbound, but that's probably fine.
    private:
        Core::Window::IWindow& m_window;
    };
}