//
// Created by Oskar.Norberg on 2025-08-26.
//

#pragma once

#include <cstdint>
#include <limits>

namespace RNGOEngine::Core::Renderer
{
    // # Low level renderer IDs

    // Buffers
    using VAO = uint32_t;
    using VBO = uint32_t;
    using EBO = uint32_t;

    constexpr auto INVALID_VAO = std::numeric_limits<VAO>::max();
    constexpr auto INVALID_VBO = std::numeric_limits<VBO>::max();
    constexpr auto INVALID_EBO = std::numeric_limits<EBO>::max();

    // Shaders
    enum class ShaderType { Vertex, Fragment };

    using ShaderID = unsigned int;
    using ShaderProgramID = unsigned int;
    using TextureID = unsigned int;

    constexpr auto INVALID_SHADER_ID = std::numeric_limits<ShaderID>::max();
    constexpr auto INVALID_SHADER_PROGRAM_ID = std::numeric_limits<ShaderProgramID>::max();
    constexpr auto INVALID_TEXTURE_ID = std::numeric_limits<TextureID>::max();

    // FrameBuffers
    using FrameBufferID = unsigned int;
    constexpr auto INVALID_FRAMEBUFFER_ID = std::numeric_limits<FrameBufferID>::max();

    enum class FrameBufferAttachmentPoint
    {
        DEPTH_ATTACHMENT = 0,
        STENCIL_ATTACHMENT = 1,

        COLOR_ATTACHMENT0 = 2,
        COLOR_ATTACHMENT1 = 3,
        COLOR_ATTACHMENT2 = 4,
        COLOR_ATTACHMENT3 = 5,
        COLOR_ATTACHMENT4 = 6,
        COLOR_ATTACHMENT5 = 7,
        // Add more if required.
        // GLAD reports supporting 31.
    };

    // RenderBuffers
    using RenderBufferID = unsigned int;
    constexpr auto INVALID_RENDERBUFFER_ID = std::numeric_limits<RenderBufferID>::max();

    enum class RenderBufferFormat
    {
        DEPTH24_STENCIL8,
        DEPTH32F_STENCIL8,
    };
}