//
// Created by Oskar.Norberg on 2025-08-25.
//

#include "Renderer/GLFW/GLFWRenderer.h"

#include <glad/gl.h>
#include <GLFW/glfw3.h>

#include <format>

#include "Data/MeshData.h"
#include "Utilities/RNGOAsserts.h"

namespace RNGOEngine::Core::Renderer
{
    GLFWRenderer::GLFWRenderer()
    {
        // TODO: Hardcoded to glfw.
        if (!gladLoadGL(glfwGetProcAddress))
        {
            RNGO_ASSERT(false && "Failed to initialize GLAD");
        }

        glEnable(GL_DEPTH_TEST);
    }

    void GLFWRenderer::SetViewPortSize(const int width, const int height)
    {
        glViewport(0, 0, width, height);
    }

    void GLFWRenderer::SetClearColor(const float r, const float g, const float b, const float a)
    {
        glClearColor(r, g, b, a);
    }

    void GLFWRenderer::ClearColor()
    {
        glClear(GL_COLOR_BUFFER_BIT);
    }

    void GLFWRenderer::ClearDepth()
    {
        glClear(GL_DEPTH_BUFFER_BIT);
    }

    void GLFWRenderer::DrawElement(const size_t numIndices)
    {
        // TODO: Scary cast.
        glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_INT, nullptr);
    }

    VAO GLFWRenderer::CreateVAO()
    {
        unsigned int vao;
        glGenVertexArrays(1, &vao);
        return vao;
    }

    VBO GLFWRenderer::CreateVBO()
    {
        unsigned vbo;
        glGenBuffers(1, &vbo);
        return vbo;
    }

    EBO GLFWRenderer::CreateEBO()
    {
        unsigned ebo;
        glGenBuffers(1, &ebo);
        return ebo;
    }

    void GLFWRenderer::DestroyVAO(const VAO vao)
    {
        glDeleteVertexArrays(1, &vao);
    }

    void GLFWRenderer::DestroyVBO(const VBO vbo)
    {
        glDeleteBuffers(1, &vbo);
    }

    void GLFWRenderer::DestroyEBO(const EBO ebo)
    {
        glDeleteBuffers(1, &ebo);
    }

    void GLFWRenderer::BindToVAO(const VAO vao)
    {
        glBindVertexArray(vao);
    }

    void GLFWRenderer::BindToVBO(const VBO vbo)
    {
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
    }

    void GLFWRenderer::BindToEBO(const EBO ebo)
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    }

    void GLFWRenderer::SetAttributePointer(const unsigned index, const int size, const size_t stride,
                                           const size_t offset)
    {
        glVertexAttribPointer(index, size, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<void*>(offset));
        glEnableVertexAttribArray(index);
    }

    void GLFWRenderer::BufferVBOData(const std::span<const std::byte> data, const bool isDynamic)
    {
        const auto usage = isDynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW;
        // TODO: Scary cast.
        glBufferData(GL_ARRAY_BUFFER, data.size_bytes(), data.data(), usage);
    }

    void GLFWRenderer::BufferEBOData(const std::span<const std::byte> data, const bool isDynamic)
    {
        const auto usage = isDynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW;
        // TODO: Scary cast.
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, data.size_bytes(), data.data(), usage);
    }

    ShaderID GLFWRenderer::CreateShader(const std::string_view source, const ShaderType type)
    {
        const auto shaderType = type == ShaderType::Vertex ? GL_VERTEX_SHADER : GL_FRAGMENT_SHADER;
        const auto shaderID = glCreateShader(shaderType);
        const char* sourcePtr = source.data();
        glShaderSource(shaderID, 1, &sourcePtr, nullptr);
        glCompileShader(shaderID);
        // TODO: Error handling.

        return shaderID;
    }

    ShaderProgramID GLFWRenderer::CreateShaderProgram(const ShaderID vertexShader,
                                                      const ShaderID fragmentShader)
    {
        const auto shaderProgram = glCreateProgram();
        glAttachShader(shaderProgram, vertexShader);
        glAttachShader(shaderProgram, fragmentShader);
        glLinkProgram(shaderProgram);
        // TODO: Error handling.

        return shaderProgram;
    }

    void GLFWRenderer::DestroyShader(const ShaderID shader)
    {
        glDeleteShader(shader);
    }

    void GLFWRenderer::DestroyShaderProgram(const ShaderProgramID program)
    {
        glDeleteProgram(program);
    }

    void GLFWRenderer::BindShaderProgram(ShaderProgramID program)
    {
        glUseProgram(program);
    }

    void GLFWRenderer::SetBool(const std::string_view name, const bool value)
    {
        // TODO: Cache the current program?
        GLint currentProgram = 0;
        glGetIntegerv(GL_CURRENT_PROGRAM, &currentProgram);
        glUniform1i(glGetUniformLocation(currentProgram, name.data()), static_cast<int>(value));
    }

    void GLFWRenderer::SetInt(const std::string_view name, const int value)
    {
        GLint currentProgram = 0;
        glGetIntegerv(GL_CURRENT_PROGRAM, &currentProgram);
        glUniform1i(glGetUniformLocation(currentProgram, name.data()), value);
    }

    void GLFWRenderer::SetFloat(const std::string_view name, const float value)
    {
        GLint currentProgram = 0;
        glGetIntegerv(GL_CURRENT_PROGRAM, &currentProgram);
        glUniform1f(glGetUniformLocation(currentProgram, name.data()), value);
    }

    void GLFWRenderer::SetVec2(const std::string_view name, const std::span<const float, 2> value)
    {
        GLint currentProgram = 0;
        glGetIntegerv(GL_CURRENT_PROGRAM, &currentProgram);
        glUniform2fv(glGetUniformLocation(currentProgram, name.data()), 1, value.data());
    }

    void GLFWRenderer::SetVec3(const std::string_view name, const std::span<const float, 3> value)
    {
        GLint currentProgram = 0;
        glGetIntegerv(GL_CURRENT_PROGRAM, &currentProgram);
        glUniform3fv(glGetUniformLocation(currentProgram, name.data()), 1, value.data());
    }

    void GLFWRenderer::SetVec4(const std::string_view name, const std::span<const float, 4> value)
    {
        GLint currentProgram = 0;
        glGetIntegerv(GL_CURRENT_PROGRAM, &currentProgram);
        glUniform4fv(glGetUniformLocation(currentProgram, name.data()), 1, value.data());
    }

    void GLFWRenderer::SetMat2(std::string_view name, std::span<const float, 4> value)
    {
        GLint currentProgram = 0;
        glGetIntegerv(GL_CURRENT_PROGRAM, &currentProgram);
        glUniformMatrix2fv(glGetUniformLocation(currentProgram, name.data()), 1, GL_FALSE, value.data());
    }

    void GLFWRenderer::SetMat3(const std::string_view name, const std::span<const float, 9> value)
    {
        GLint currentProgram = 0;
        glGetIntegerv(GL_CURRENT_PROGRAM, &currentProgram);
        glUniformMatrix3fv(glGetUniformLocation(currentProgram, name.data()), 1, GL_FALSE, value.data());
    }

    void GLFWRenderer::SetMat4(const std::string_view name, const std::span<const float, 16> value)
    {
        GLint currentProgram = 0;
        glGetIntegerv(GL_CURRENT_PROGRAM, &currentProgram);
        glUniformMatrix4fv(glGetUniformLocation(currentProgram, name.data()), 1, GL_FALSE, value.data());
    }

    void GLFWRenderer::SetTexture(const std::string_view name, const TextureID texture, const unsigned slot)
    {
        GLint currentProgram = 0;
        glGetIntegerv(GL_CURRENT_PROGRAM, &currentProgram);

        const auto uniformLocation = glGetUniformLocation(
            currentProgram, name.data());
        if (uniformLocation == -1)
        {
            RNGO_ASSERT(false && "Texture Uniform not found in shader.");
            return;
        }

        glActiveTexture(GL_TEXTURE0 + slot);
        glBindTexture(GL_TEXTURE_2D, texture);
        glUniform1i(uniformLocation, slot);
    }

    TextureID GLFWRenderer::CreateTexture(const unsigned int width, const unsigned int height,
                                          const unsigned int nrChannels,
                                          const std::span<const std::byte> data)
    {
        unsigned int textureHandle;
        glGenTextures(1, &textureHandle);
        glBindTexture(GL_TEXTURE_2D, textureHandle);

        // TODO: These should probably be passed in as parameters.
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        const auto* textureData = data.data();
        if (nrChannels == 4)
        {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, textureData);
        }
        else if (nrChannels == 3)
        {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, textureData);
        }
        else
        {
            RNGO_ASSERT(false && "Unsupported number of channels in texture");
            return INVALID_TEXTURE_ID;
        }

        glGenerateMipmap(GL_TEXTURE_2D);

        return textureHandle;
    }

    void GLFWRenderer::DestroyTexture(const TextureID texture)
    {
        glDeleteTextures(1, &texture);
    }
}