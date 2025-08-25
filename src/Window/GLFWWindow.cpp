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
    }

    GLFWWindow::~GLFWWindow()
    {
        glfwDestroyWindow(m_window);
        glfwTerminate();
    }

    void GLFWWindow::PollEvents(Events::EventQueue& eventQueue)
    {
        glfwPollEvents();

        if (glfwWindowShouldClose(m_window))
        {
            eventQueue.EmplaceEvent<Events::ExitEvent>();
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
}