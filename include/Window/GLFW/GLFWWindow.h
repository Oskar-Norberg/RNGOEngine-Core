//
// Created by Oskar.Norberg on 2025-08-25.
//

#pragma once

#include "GLFW/glfw3.h"
#include "Window/IWindow.h"

namespace RNGOEngine::Core::Window
{
    class GLFWWindow : public IWindow
    {
    public:
        GLFWWindow(int width, int height, std::string_view name);
        ~GLFWWindow() override;

    public:
        void PollEvents(Events::EventQueue& eventQueue) override;
        void SwapBuffers() override;
        void SetName(std::string_view name) override;

    private:
        GLFWwindow* m_window;

        char lastFrameKeys[GLFW_KEY_LAST + 1];
        bool keySetInitialized = false;

    private:
        void PollKeyboardEvents(Events::EventQueue& eventQueue);
    };
}