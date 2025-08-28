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
        void PollWindowEvents(Events::EventQueue& eventQueue) override;
        void PollKeyboardEvents(Events::EventQueue& eventQueue) override;
        void PollMouseEvents(Events::EventQueue& eventQueue) override;

        void SwapBuffers() override;
        void SetName(std::string_view name) override;

    private:
        GLFWwindow* m_window;

    private:
        double m_lastMouseX, m_lastMouseY;
    };
}