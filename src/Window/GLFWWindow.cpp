//
// Created by Oskar.Norberg on 2025-08-25.
//

// TODO: much badness
#include <cassert>

#include "Window/GLFW/GLFWWindow.h"
#include "EventQueue/EngineEvents/EngineEvents.h"

namespace RNGOEngine::Core::Window
{
    GLFWWindow::GLFWWindow(int width, int height, std::string_view name)
        : m_window(nullptr),
          m_width(width),
          m_height(height),
          m_hasPendingResize(false),
          m_lastMouseX(0.0),
          m_lastMouseY(0.0)
    {
        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        m_window = glfwCreateWindow(width, height, name.data(), nullptr, nullptr);

        // TODO: PLEASE MAKE CUSTOM ENGINE LEVEL ASSERTIONS.
        assert(m_window && "Failed to create GLFW window");

        // TODO: Not sure this should be done here.
        glfwMakeContextCurrent(m_window);
        glfwSetWindowUserPointer(m_window, this);

        glfwSetFramebufferSizeCallback(m_window, [](GLFWwindow* glfwWindow, int width, int height)
        {
            auto* userPtr = glfwGetWindowUserPointer(glfwWindow);

            if (userPtr)
            {
                static_cast<GLFWWindow*>(userPtr)->WindowSizeCallback(width, height);
            }
            else
            {
                assert(false && "GLFW window user pointer is null.");
            }
        });

        glfwSetKeyCallback(m_window, [](GLFWwindow* glfwWindow, int key, int scancode, int action, int mods)
        {
            auto* userPtr = glfwGetWindowUserPointer(glfwWindow);

            if (userPtr)
            {
                static_cast<GLFWWindow*>(userPtr)->KeyEventCallback(key, scancode, action, mods);
            }
            else
            {
                assert(false && "GLFW window user pointer is null.");
            }
        });
    }

    GLFWWindow::~GLFWWindow()
    {
        glfwDestroyWindow(m_window);
        glfwTerminate();
    }

    void GLFWWindow::PollWindowEvents(Events::EventQueue& eventQueue)
    {
        glfwPollEvents();

        if (glfwWindowShouldClose(m_window))
        {
            eventQueue.EmplaceEvent<Events::ExitEvent>();
        }

        if (m_hasPendingResize)
        {
            eventQueue.EmplaceEvent<Events::WindowSizeEvent>(m_width, m_height);
        }
    }

    void GLFWWindow::SwapBuffers()
    {
        glfwSwapBuffers(m_window);
    }

    void GLFWWindow::SetName(std::string_view name)
    {
        glfwSetWindowTitle(m_window, name.data());
    }

    void GLFWWindow::PollKeyboardEvents(Events::EventQueue& eventQueue)
    {
        eventQueue.PushEventMultiple(m_keyEvents);
        m_keyEvents.clear();
    }

    void GLFWWindow::PollMouseEvents(Events::EventQueue& eventQueue)
    {
        double mouseX, mouseY;
        glfwGetCursorPos(m_window, &mouseX, &mouseY);

        double mouseDiffX = mouseX - m_lastMouseX;
        double mouseDiffY = mouseY - m_lastMouseY;

        if (mouseDiffX != 0 || mouseDiffY != 0)
        {
            eventQueue.EmplaceEvent<Events::MouseEvent>(mouseDiffX, mouseDiffY);
        }

        m_lastMouseX = mouseX;
        m_lastMouseY = mouseY;
    }

    bool GLFWWindow::ListenSendEvents(Events::EventQueue& eventQueue)
    {
        // No events to listen/send yet.
        return false;
    }

    void GLFWWindow::WindowSizeCallback(int width, int height)
    {
        m_hasPendingResize = true;
        m_width = width;
        m_height = height;
    }

    void GLFWWindow::KeyEventCallback(int key, int scancode, int action, int mods)
    {
        m_keyEvents.emplace_back(key, action == GLFW_PRESS
                                          ? Events::KeyAction::Press
                                          : Events::KeyAction::Release);
    }
}