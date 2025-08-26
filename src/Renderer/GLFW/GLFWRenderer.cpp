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

    MeshHandle GLFWRenderer::CreateMesh(std::span<float> vertices, std::span<unsigned> indices)
    {
        unsigned int VBO;
        glGenBuffers(1, &VBO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vertices.size_bytes(), vertices.data(), GL_STATIC_DRAW);

        unsigned int EBO;
        glGenBuffers(1, &EBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size_bytes(), indices.data(), GL_STATIC_DRAW);

        unsigned int VAO;
        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), static_cast<void*>(nullptr));

        return VAO;
    }

    ShaderHandle GLFWRenderer::CreateShader(std::string_view vertexSource, std::string_view fragmentSource)
    {
        //  Compile Vertex Shader
        auto vertexShader = glCreateShader(GL_VERTEX_SHADER);
        const char* vertexSourcePtr = vertexSource.data();
        glShaderSource(vertexShader, 1, &vertexSourcePtr, nullptr);
        glCompileShader(vertexShader);
        CheckCompilationErrors(vertexShader);

        // Compile Fragment Shader
        auto fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        const char* fragmentSourcePtr = fragmentSource.data();
        glShaderSource(fragmentShader, 1, &fragmentSourcePtr, nullptr);
        glCompileShader(fragmentShader);
        CheckCompilationErrors(fragmentShader);

        auto shaderProgram = glCreateProgram();
        glAttachShader(shaderProgram, vertexShader);
        glAttachShader(shaderProgram, fragmentShader);
        glLinkProgram(shaderProgram);
        CheckLinkingErrors(shaderProgram);

        return shaderProgram;
    }

    bool GLFWRenderer::CheckCompilationErrors(unsigned int shader)
    {
        int success;
        char infoLog[512];
        
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            assert(false && infoLog);
            return false;
        }

        return true;
    }

    bool GLFWRenderer::CheckLinkingErrors(unsigned int program)
    {
        int success;
        char infoLog[512];
        
        glGetShaderiv(program, GL_LINK_STATUS, &success);
        if (!success)
        {
            assert(false && infoLog);
            return false;
        }

        return true;
    }
}