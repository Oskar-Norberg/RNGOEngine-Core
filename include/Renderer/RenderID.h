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

    // TODO: Consider moving to a separate file?
    // # Higher level renderer IDs
    using MeshID = unsigned int;
    using MaterialID = unsigned int;

    constexpr auto INVALID_MESH_ID = std::numeric_limits<MeshID>::max();
    constexpr auto INVALID_MATERIAL_ID = std::numeric_limits<MaterialID>::max();

}