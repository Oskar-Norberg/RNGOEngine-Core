//
// Created by Oskar.Norberg on 2025-08-25.
//

#pragma once

#include "EventQueue/EngineEvents/EngineEvents.h"
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

    public:
        bool ListenSendEvents(Events::EventQueue& eventQueue) override;

        void SwapBuffers() override;
        void SetName(std::string_view name) override;

    public:
        int GetWidth() const override
        {
            return m_width;
        }

        int GetHeight() const override
        {
            return m_height;
        }

    private:
        GLFWwindow* m_window;
        int m_width, m_height;
        bool m_hasPendingResize;

    private:
        double m_accumulatedX, m_accumulatedY;
        double m_lastMouseX, m_lastMouseY;

    private:
        std::vector<Events::KeyEvent> m_keyEvents;
        std::vector<Events::MouseButtonEvent> m_mouseButtonEvents;

    private:
        void WindowSizeCallback(int width, int height);
        void KeyEventCallback(int key, int scancode, int action, int mods);
        void MouseMoveCallback(double x, double y);
        void MouseButtonCallback(int button, int action, int mods);
    };
}