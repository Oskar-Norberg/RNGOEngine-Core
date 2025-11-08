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
    }

    void GLFWRenderer::EnableFeature(RenderFeature feature)
    {
        glEnable(GetGLFeature(feature));
    }

    void GLFWRenderer::DisableFeature(RenderFeature feature)
    {
        glDisable(GetGLFeature(feature));
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

    void GLFWRenderer::SetBool(ShaderProgramID shader, std::string_view name, bool value)
    {
        glUniform1i(glGetUniformLocation(shader, name.data()), static_cast<int>(value));
    }

    void GLFWRenderer::SetInt(ShaderProgramID shader, std::string_view name, int value)
    {
        glUniform1i(glGetUniformLocation(shader, name.data()), value);
    }

    void GLFWRenderer::SetFloat(ShaderProgramID shader, std::string_view name, float value)
    {
        glUniform1f(glGetUniformLocation(shader, name.data()), value);
    }

    void GLFWRenderer::SetVec2(ShaderProgramID shader, std::string_view name, std::span<const float, 2> value)
    {
        glUniform2fv(glGetUniformLocation(shader, name.data()), 1, value.data());
    }

    void GLFWRenderer::SetVec3(ShaderProgramID shader, std::string_view name, std::span<const float, 3> value)
    {
        glUniform3fv(glGetUniformLocation(shader, name.data()), 1, value.data());
    }

    void GLFWRenderer::SetVec4(ShaderProgramID shader, std::string_view name, std::span<const float, 4> value)
    {
        glUniform4fv(glGetUniformLocation(shader, name.data()), 1, value.data());
    }

    void GLFWRenderer::SetMat2(ShaderProgramID shader, std::string_view name, std::span<const float, 4> value)
    {
        glUniformMatrix2fv(glGetUniformLocation(shader, name.data()), 1, GL_FALSE, value.data());
    }

    void GLFWRenderer::SetMat3(ShaderProgramID shader, std::string_view name, std::span<const float, 9> value)
    {
        glUniformMatrix3fv(glGetUniformLocation(shader, name.data()), 1, GL_FALSE, value.data());
    }

    void GLFWRenderer::SetMat4(ShaderProgramID shader, std::string_view name,
                               std::span<const float, 16> value)
    {
        glUniformMatrix4fv(glGetUniformLocation(shader, name.data()), 1, GL_FALSE, value.data());
    }

    void GLFWRenderer::SetTexture(ShaderProgramID shader, std::string_view name, unsigned slot)
    {
        glActiveTexture(GL_TEXTURE0 + slot);
        glUniform1i(glGetUniformLocation(shader, name.data()), slot);
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

    void GLFWRenderer::BindTexture(const TextureID texture, unsigned slot)
    {
        glBindTexture(GL_TEXTURE_2D, texture);
    }

    FrameBufferID GLFWRenderer::CreateFrameBuffer()
    {
        unsigned int framebuffer;
        glGenFramebuffers(1, &framebuffer);
        return framebuffer;
    }

    void GLFWRenderer::DestroyFrameBuffer(const FrameBufferID framebuffer)
    {
        glDeleteFramebuffers(1, &framebuffer);
    }

    void GLFWRenderer::BindFrameBuffer(const FrameBufferID frameBuffer)
    {
        glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
    }

    void GLFWRenderer::AttachTextureToFrameBuffer(const TextureID texture,
                                                  const FrameBufferAttachmentPoint attachmentPoint)
    {
        const unsigned int glAttachmentPoint = GetGLAttachmentPoint(attachmentPoint);

        // TODO: Assumes 2D, would this ever be different?
        glFramebufferTexture2D(GL_FRAMEBUFFER, glAttachmentPoint, GL_TEXTURE_2D, texture, 0);
    }

    RenderBufferID GLFWRenderer::CreateRenderBuffer(const RenderBufferFormat format, const unsigned width,
                                                    const unsigned height)
    {
        unsigned int rbo;
        glGenRenderbuffers(1, &rbo);

        const auto glRenderBufferFormat = GetGLRenderBufferFormat(format);
        glRenderbufferStorage(GL_RENDERBUFFER, glRenderBufferFormat, width, height);

        return rbo;
    }

    void GLFWRenderer::DestroyRenderBuffer(const RenderBufferID renderBuffer)
    {
        glDeleteRenderbuffers(1, &renderBuffer);
    }

    void GLFWRenderer::BindRenderBuffer(RenderBufferID renderBuffer)
    {
        glBindRenderbuffer(GL_RENDERBUFFER, renderBuffer);
    }

    void GLFWRenderer::AttachRenderBufferToFrameBuffer(RenderBufferID renderBuffer,
                                                       FrameBufferAttachmentPoint attachmentPoint)
    {
        const unsigned int glAttachmentPoint = GetGLAttachmentPoint(attachmentPoint);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, glAttachmentPoint, GL_RENDERBUFFER, renderBuffer);
    }

    unsigned int GLFWRenderer::GetGLFeature(RenderFeature feature)
    {
        unsigned int glFeature = 0;

        // TODO: I forgor about |= when writing this.
        if (DepthTesting & feature)
        {
            glFeature = glFeature | GL_DEPTH_TEST;
        }

        if (Blending & feature)
        {
            glFeature = glFeature | GL_BLEND;
        }

        if (BackFaceCulling & feature)
        {
            glFeature = glFeature | GL_CULL_FACE;
        }

        return glFeature;
    }

    unsigned int GLFWRenderer::GetGLAttachmentPoint(FrameBufferAttachmentPoint attachmentPoint)
    {
        switch (attachmentPoint)
        {
            case FrameBufferAttachmentPoint::DEPTH_ATTACHMENT:
                return GL_DEPTH_ATTACHMENT;
            case FrameBufferAttachmentPoint::STENCIL_ATTACHMENT:
                return GL_STENCIL_ATTACHMENT;
            case FrameBufferAttachmentPoint::COLOR_ATTACHMENT0:
                return GL_COLOR_ATTACHMENT0;
            case FrameBufferAttachmentPoint::COLOR_ATTACHMENT1:
                return GL_COLOR_ATTACHMENT1;
            case FrameBufferAttachmentPoint::COLOR_ATTACHMENT2:
                return GL_COLOR_ATTACHMENT2;
            case FrameBufferAttachmentPoint::COLOR_ATTACHMENT3:
                return GL_COLOR_ATTACHMENT3;
            case FrameBufferAttachmentPoint::COLOR_ATTACHMENT4:
                return GL_COLOR_ATTACHMENT4;
            case FrameBufferAttachmentPoint::COLOR_ATTACHMENT5:
                return GL_COLOR_ATTACHMENT5;
            default:
                RNGO_ASSERT(
                    false &&
                    "GLFWRenderer::AttachTextureToFrameBuffer - Unsupported FrameBufferAttachmentPoint");
        }
    }

    unsigned int GLFWRenderer::GetGLRenderBufferFormat(RenderBufferFormat renderBufferFormat)
    {
        switch (renderBufferFormat)
        {
            case RenderBufferFormat::DEPTH24_STENCIL8:
                return GL_DEPTH24_STENCIL8;
            case RenderBufferFormat::DEPTH32F_STENCIL8:
                return GL_DEPTH32F_STENCIL8;
            default:
                RNGO_ASSERT(
                    false && "GLFWRenderer::GetGLRenderBufferFormat - Unsupported RenderBufferFormat");
        }
    }
}