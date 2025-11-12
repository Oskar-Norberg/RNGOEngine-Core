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

    void GLFWRenderer::EnableFeature(const RenderFeature feature)
    {
        EnableFeatures(feature);
    }

    void GLFWRenderer::DisableFeature(const RenderFeature feature)
    {
        DisableFeatures(feature);
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

        int success;
        static char ShaderCompileInfoLog[512];
        glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);

        if (success == GL_FALSE)
        {
            glGetShaderInfoLog(shaderID, 512, nullptr, ShaderCompileInfoLog);
            RNGO_ASSERT(false && "GLFWRenderer::CreateShader - Shader compilation failed.");
        }

        return shaderID;
    }

    ShaderProgramID GLFWRenderer::CreateShaderProgram(const ShaderID vertexShader,
                                                      const ShaderID fragmentShader)
    {
        const auto shaderProgram = glCreateProgram();
        glAttachShader(shaderProgram, vertexShader);
        glAttachShader(shaderProgram, fragmentShader);
        glLinkProgram(shaderProgram);
        
        int success;
        static char ShaderProgramLinkInfoLog[512];
        glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
        if (success == GL_FALSE)
        {
            // TODO: Return expected?
            glGetProgramInfoLog(shaderProgram, 512, nullptr, ShaderProgramLinkInfoLog);
            RNGO_ASSERT(false && "GLFWRenderer::CreateShaderProgram - Shader program linking failed.");
        }

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

    TextureID GLFWRenderer::CreateTexture2D(const Texture2DProperties properties, const int width, const int height,
        const std::span<const std::byte> data)
    {
        unsigned int textureHandle;
        glGenTextures(1, &textureHandle);
        glBindTexture(GL_TEXTURE_2D, textureHandle);

        // Wrapping mode same for both axis for now.
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GetGLTextureWrapping(properties.WrappingMode));
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GetGLTextureWrapping(properties.WrappingMode));

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                        GetGLTextureFiltering(properties.MinifyingFilter));
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
                        GetGLTextureFiltering(properties.MagnifyingFilter));

        const auto* textureData = data.data();

        switch (properties.Format)
        {
            case TextureFormat::RGBA:
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE,
                             textureData);
                break;
            case TextureFormat::RGB:
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE,
                             textureData);
                break;
            case TextureFormat::DEPTH24_STENCIL8:
                glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, width, height, 0, GL_DEPTH_STENCIL,
                             GL_UNSIGNED_INT_24_8,
                             textureData);
                break;
            case TextureFormat::DEPTH32F_STENCIL8:
                glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH32F_STENCIL8, width, height, 0, GL_DEPTH_STENCIL,
                             GL_FLOAT_32_UNSIGNED_INT_24_8_REV,
                             textureData);
                break;
            default:
                RNGO_ASSERT(false && "GLFWRenderer::CreateTexture2D - Unsupported TextureFormat");
        }

        if (GetGLUsingMipMaps(properties.MinifyingFilter, properties.MagnifyingFilter))
        {
            glGenerateMipmap(GL_TEXTURE_2D);
        }

        return textureHandle;
    }

    void GLFWRenderer::DestroyTexture(const TextureID texture)
    {
        glDeleteTextures(1, &texture);
    }

    void GLFWRenderer::BindTexture(const TextureID texture, unsigned slot)
    {
        glActiveTexture(GL_TEXTURE0 + slot);
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

        glBindRenderbuffer(GL_RENDERBUFFER, rbo);

        const auto glRenderBufferFormat = GetGLRenderBufferFormat(format);
        glRenderbufferStorage(GL_RENDERBUFFER, glRenderBufferFormat, width, height);

        // TODO: A lot of these functions should really unbind at the end.

        return rbo;
    }

    void GLFWRenderer::DestroyRenderBuffer(const RenderBufferID renderBuffer)
    {
        glDeleteRenderbuffers(1, &renderBuffer);
    }

    void GLFWRenderer::BindRenderBuffer(const RenderBufferID renderBuffer)
    {
        glBindRenderbuffer(GL_RENDERBUFFER, renderBuffer);
    }

    FrameBufferStatus GLFWRenderer::GetFrameBufferStatus()
    {
        return GetFrameBufferStatusFromGL(glCheckFramebufferStatus(GL_FRAMEBUFFER));
    }

    void GLFWRenderer::AttachRenderBufferToFrameBuffer(const RenderBufferID renderBuffer,
                                                       const FrameBufferAttachmentPoint attachmentPoint)
    {
        const unsigned int glAttachmentPoint = GetGLAttachmentPoint(attachmentPoint);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, glAttachmentPoint, GL_RENDERBUFFER, renderBuffer);
    }

    void GLFWRenderer::EnableFeatures(const RenderFeature feature)
    {
        // TODO: Duplicate code.
        if (DepthTesting & feature)
        {
            glEnable(GL_DEPTH_TEST);
        }

        if (Blending & feature)
        {
            glEnable(GL_BLEND);
        }

        if (BackFaceCulling & feature)
        {
            glEnable(GL_CULL_FACE);
        }
    }

    void GLFWRenderer::DisableFeatures(const RenderFeature feature)
    {
        if (DepthTesting & feature)
        {
            glDisable(GL_DEPTH_TEST);
        }

        if (Blending & feature)
        {
            glDisable(GL_BLEND);
        }

        if (BackFaceCulling & feature)
        {
            glDisable(GL_CULL_FACE);
        }
    }

    unsigned int GLFWRenderer::GetGLTextureFiltering(const TextureFiltering filtering)
    {
        switch (filtering)
        {
            case TextureFiltering::NEAREST:
                return GL_NEAREST;
            case TextureFiltering::LINEAR:
                return GL_LINEAR;
            case TextureFiltering::NEAREST_MIPMAP_NEAREST:
                return GL_NEAREST_MIPMAP_NEAREST;
            case TextureFiltering::NEAREST_MIPMAP_LINEAR:
                return GL_NEAREST_MIPMAP_LINEAR;
            case TextureFiltering::LINEAR_MIPMAP_NEAREST:
                return GL_LINEAR_MIPMAP_NEAREST;
            case TextureFiltering::LINEAR_MIPMAP_LINEAR:
                return GL_LINEAR_MIPMAP_LINEAR;
            default:
                RNGO_ASSERT(false && "GLFWRenderer::GetGLTextureFiltering - Unsupported TextureFiltering");
        }
    }

    bool GLFWRenderer::GetGLUsingMipMaps(const TextureFiltering minifying, const TextureFiltering magnifying)
    {
        auto isMipmapped = [](const TextureFiltering filter)
        {
            return filter == TextureFiltering::LINEAR_MIPMAP_LINEAR ||
                   filter == TextureFiltering::LINEAR_MIPMAP_NEAREST ||
                   filter == TextureFiltering::NEAREST_MIPMAP_LINEAR ||
                   filter == TextureFiltering::NEAREST_MIPMAP_NEAREST;
        };

        return isMipmapped(minifying) || isMipmapped(magnifying);
    }

    unsigned int GLFWRenderer::GetGLTextureWrapping(const TextureWrapping wrapping)
    {
        switch (wrapping)
        {
            case TextureWrapping::REPEAT:
                return GL_REPEAT;
            case TextureWrapping::MIRRORED_REPEAT:
                return GL_MIRRORED_REPEAT;
            case TextureWrapping::CLAMP_TO_EDGE:
                return GL_CLAMP_TO_EDGE;
            case TextureWrapping::CLAMP_TO_BORDER:
                return GL_CLAMP_TO_BORDER;
            default:
                RNGO_ASSERT(
                    false &&
                    "GLFWRenderer::GetGLTextureWrapping - Unsupported TextureWrapping");
                return GL_REPEAT;
        }
    }

    unsigned int GLFWRenderer::GetGLAttachmentPoint(const FrameBufferAttachmentPoint attachmentPoint)
    {
        switch (attachmentPoint)
        {
            case FrameBufferAttachmentPoint::DEPTH_ATTACHMENT:
                return GL_DEPTH_ATTACHMENT;
            case FrameBufferAttachmentPoint::STENCIL_ATTACHMENT:
                return GL_STENCIL_ATTACHMENT;
            case FrameBufferAttachmentPoint::DEPTH_STENCIL_ATTACHMENT:
                return GL_DEPTH_STENCIL_ATTACHMENT;
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
                return GL_COLOR_ATTACHMENT0;
        }
    }

    unsigned int GLFWRenderer::GetGLRenderBufferFormat(const RenderBufferFormat renderBufferFormat)
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
                return GL_DEPTH24_STENCIL8;
        }
    }

    unsigned int GLFWRenderer::GetGLTextureFormat(TextureFormat format)
    {
        switch (format)
        {
            case TextureFormat::RGB:
                return GL_RGB;
            case TextureFormat::RGBA:
                return GL_RGBA;
            case TextureFormat::DEPTH24_STENCIL8:
                return GL_DEPTH24_STENCIL8;
            case TextureFormat::DEPTH32F_STENCIL8:
                return GL_DEPTH32F_STENCIL8;
            default:
                RNGO_ASSERT(
                    false && "GLFWRenderer::GetGLTextureFormat - Unsupported TextureFormat");
                return GL_RGBA;
        }
    }

    FrameBufferStatus GLFWRenderer::GetFrameBufferStatusFromGL(unsigned int status)
    {
        switch (status)
        {
            case GL_FRAMEBUFFER_COMPLETE:
                return FrameBufferStatus::COMPLETE;
            case GL_FRAMEBUFFER_UNDEFINED:
                return FrameBufferStatus::UNDEFINED;
            case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
                return FrameBufferStatus::INCOMPLETE_ATTACHMENT;
            case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
                return FrameBufferStatus::MISSING_ATTACHMENT;
            case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:
                return FrameBufferStatus::INCOMPLETE_DRAW_BUFFER;
            case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:
                return FrameBufferStatus::INCOMPLETE_READ_BUFFER;
            case GL_FRAMEBUFFER_UNSUPPORTED:
                return FrameBufferStatus::UNSUPPORTED;
            case GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE:
                return FrameBufferStatus::INCOMPLETE_MULTISAMPLE;
            case GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS:
                return FrameBufferStatus::INCOMPLETE_LAYER_TARGETS;
            default:
                return FrameBufferStatus::UNDEFINED;
        }
    }
}