//
// Created by Oskar.Norberg on 2025-08-25.
//

#include "Renderer/GLFW/GLFWRenderer.h"

#include <glad/gl.h>
#include <GLFW/glfw3.h>

#include <format>

#include "AssetManager/AssetManagers/MaterialManager.h"
#include "AssetManager/AssetManagers/TextureManager.h"
#include "Data/MeshData.h"
#include "Utilities/RNGOAsserts.h"

namespace RNGOEngine::Core::Renderer
{
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

    void GLFWRenderer::BufferVBOData(const std::span<std::byte> data, const bool isDynamic)
    {
        const auto usage = isDynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW;
        // TODO: Scary cast.
        glBufferData(GL_ARRAY_BUFFER, data.size_bytes(), data.data(), usage);
    }

    void GLFWRenderer::BufferEBOData(const std::span<std::byte> data, const bool isDynamic)
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

    ShaderProgramID GLFWRenderer::CreateShaderProgram(const ShaderID vertexShader, const ShaderID fragmentShader)
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

    TextureID GLFWRenderer::CreateTexture(AssetHandling::Textures::TextureHandle textureHandle)
    {
        // TODO:
        return INVALID_TEXTURE_ID;
    }

    void GLFWRenderer::DestroyTexture(TextureID texture)
    {
        // TODO:
    }
}