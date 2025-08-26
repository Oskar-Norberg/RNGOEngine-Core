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

        // Render Opaques
        for (const auto& opaqueDrawable : m_drawQueue.opaqueObjects)
        {
            glUseProgram(opaqueDrawable.shader);
            glBindVertexArray(opaqueDrawable.mesh);

            assert(m_meshSpecifications.contains(opaqueDrawable.mesh) && "Mesh not found in specifications");

            const auto& meshSpec = m_meshSpecifications[opaqueDrawable.mesh];
            glDrawElements(GL_TRIANGLES, meshSpec.nrOfVertices + meshSpec.nrOfIndices, GL_UNSIGNED_INT, 0);
        }
    }

    MeshHandle GLFWRenderer::CreateMesh(std::span<float> vertices, std::span<unsigned> indices)
    {
        unsigned int VAO, VBO, EBO;
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vertices.size_bytes(), vertices.data(), GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size_bytes(), indices.data(), GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), static_cast<void*>(nullptr));
        glEnableVertexAttribArray(0);

        m_meshSpecifications.insert(
            std::make_pair(VAO, MeshSpecification
                           {
                               .nrOfVertices = static_cast<unsigned int>(vertices.size() / 3),
                               .nrOfIndices = static_cast<unsigned int>(indices.size() / 3)
                           }
            ));

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

    void GLFWRenderer::SetBool(ShaderHandle shader, std::string_view name, bool value)
    {
        glUseProgram(shader);
        glUniform1i(glGetUniformLocation(shader, name.data()), static_cast<int>(value));
    }

    void GLFWRenderer::SetInt(ShaderHandle shader, std::string_view name, int value)
    {
        glUseProgram(shader);
        glUniform1i(glGetUniformLocation(shader, name.data()), value);
    }

    void GLFWRenderer::SetFloat(ShaderHandle shader, std::string_view name, float value)
    {
        glUseProgram(shader);
        glUniform1f(glGetUniformLocation(shader, name.data()), value);
    }

    void GLFWRenderer::SetVec2(ShaderHandle shader, std::string_view name, const glm::vec2& value)
    {
        glUseProgram(shader);
        glUniform2fv(glGetUniformLocation(shader, name.data()), 1, &value[0]);
    }

    void GLFWRenderer::SetVec3(ShaderHandle shader, std::string_view name, const glm::vec3& value)
    {
        glUseProgram(shader);
        glUniform3fv(glGetUniformLocation(shader, name.data()), 1, &value[0]);
    }

    void GLFWRenderer::SetVec4(ShaderHandle shader, std::string_view name, const glm::vec4& value)
    {
        glUseProgram(shader);
        glUniform4fv(glGetUniformLocation(shader, name.data()), 1, &value[0]);
    }

    void GLFWRenderer::SetMat4(ShaderHandle shader, std::string_view name, const glm::mat4& value)
    {
        glUseProgram(shader);
        glUniformMatrix4fv(glGetUniformLocation(shader, name.data()), 1, GL_FALSE, &value[0][0]);
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