//
// Created by Oskar.Norberg on 2025-08-25.
//

// TODO: much badness
#include <cassert>
#include <GLFW/glfw3.h>

#include "Window/GLFW/GLFWWindow.h"
#include "EventQueue/EngineEvents/EngineEvents.h"

namespace RNGOEngine::Core::Window
{
    GLFWWindow::GLFWWindow(int width, int height, std::string_view name)
        : m_window(nullptr),
          m_width(width),
          m_height(height),
          m_hasPendingResize(false),
          m_accumulatedX(0.0),
          m_accumulatedY(0.0),
          m_lastMouseX(0.0),
          m_lastMouseY(0.0)
    {
        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        // TODO: Hardcoded to OpenGL core profile. Make configurable.
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwSwapInterval(0);

        m_window = glfwCreateWindow(width, height, name.data(), nullptr, nullptr);

        // TODO: PLEASE MAKE CUSTOM ENGINE LEVEL ASSERTIONS.
        assert(m_window && "Failed to create GLFW window");

        glfwMakeContextCurrent(m_window);
        glfwSetWindowUserPointer(m_window, this);

        glfwSetFramebufferSizeCallback(m_window, [](GLFWwindow* glfwWindow, int width, int height)
        {
            if (auto* userPtr = glfwGetWindowUserPointer(glfwWindow))
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
            if (auto* userPtr = glfwGetWindowUserPointer(glfwWindow))
            {
                static_cast<GLFWWindow*>(userPtr)->KeyEventCallback(key, scancode, action, mods);
            }
            else
            {
                assert(false && "GLFW window user pointer is null.");
            }
        });

        glfwSetCursorPosCallback(m_window, [](GLFWwindow* glfwWindow, double x, double y)
        {
            if (auto* userPtr = glfwGetWindowUserPointer(glfwWindow))
            {
                static_cast<GLFWWindow*>(userPtr)->MouseMoveCallback(x, y);
            }
            else
            {
                assert(false && "GLFW window user pointer is null.");
            }
        });

        glfwSetMouseButtonCallback(m_window, [](GLFWwindow* glfwWindow, int button, int action, int mods)
        {
            if (auto* userPtr = glfwGetWindowUserPointer(glfwWindow))
            {
                static_cast<GLFWWindow*>(userPtr)->MouseButtonCallback(button, action, mods);
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
        eventQueue.EmplaceEvent<Events::MouseMoveEvent>(m_accumulatedX, m_accumulatedY);

        m_accumulatedX = 0.0f;
        m_accumulatedY = 0.0f;

        if (!m_mouseButtonEvents.empty())
        {
            eventQueue.PushEventMultiple<Events::MouseButtonEvent>(m_mouseButtonEvents);
            m_mouseButtonEvents.clear();
        }
    }

    bool GLFWWindow::ListenSendEvents(Events::EventQueue& eventQueue)
    {
        // No events to listen/send yet.
        return false;
    }

    void GLFWWindow::PollGameEvents(Events::EventQueue& eventQueue)
    {
        const auto mouseModeEvents = eventQueue.GetEvents<Events::MouseModeEvent>();
        for (const auto& [mouseMode] : mouseModeEvents)
        {
            switch (mouseMode)
            {
                case Data::Mouse::MouseMode::Normal:
                    glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
                    break;
                    
                case Data::Mouse::MouseMode::Hidden:
                    glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
                    break;
                    
                case Data::Mouse::MouseMode::Locked:
                    glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
                    break;
                    
                default:
                    break;
            }
        }
    }

    void GLFWWindow::WindowSizeCallback(int width, int height)
    {
        m_hasPendingResize = true;
        m_width = width;
        m_height = height;
    }

    void GLFWWindow::KeyEventCallback(int key, int scancode, int action, int mods)
    {
        if (action != GLFW_PRESS && action != GLFW_RELEASE)
        {
            return;
        }
        
        m_keyEvents.emplace_back(key, action == GLFW_PRESS
                                          ? Events::ButtonAction::Press
                                          : Events::ButtonAction::Release);
    }

    void GLFWWindow::MouseMoveCallback(double x, double y)
    {
        if (m_lastMouseX != 0.0 && m_lastMouseY != 0.0)
        {
            m_accumulatedX += x - m_lastMouseX;
            m_accumulatedY += y - m_lastMouseY;
        }

        m_lastMouseX = x;
        m_lastMouseY = y;
    }

    void GLFWWindow::MouseButtonCallback(int button, int action, int mods)
    {
        if (action != GLFW_PRESS && action != GLFW_RELEASE)
        {
            return;
        }
        
        m_mouseButtonEvents.emplace_back(button, action == GLFW_PRESS
                                                     ? Events::ButtonAction::Press
                                                     : Events::ButtonAction::Release);
    }
}