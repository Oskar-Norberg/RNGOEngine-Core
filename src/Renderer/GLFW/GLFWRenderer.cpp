//
// Created by Oskar.Norberg on 2025-08-25.
//

#include "Renderer/GLFW/GLFWRenderer.h"

#include <glad/gl.h>
#include <GLFW/glfw3.h>

// TODO: AAAAAAAAAAAAAAAAAAAAA
#include <cassert>

namespace RNGOEngine::Core::Renderer
{
    GLFWRenderer::GLFWRenderer()
    {
        if (!gladLoadGL(glfwGetProcAddress))
        {
            assert(false && "Failed to initialize GLAD");
        }

        glViewport(0, 0, 800, 600);
    }

    void GLFWRenderer::Render(Window::IWindow& window)
    {
        glClearColor(0.25f, 0.35f, 0.25f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
    }
}