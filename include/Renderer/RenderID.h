//
// Created by Oskar.Norberg on 2025-08-26.
//

#pragma once

#include <limits>

namespace RNGOEngine::Core::Renderer
{
    using MeshID = unsigned int;

    using ShaderID = unsigned int;
    using ShaderProgramID = unsigned int;
    using MaterialID = unsigned int;

    using TextureID = unsigned int;

    constexpr auto INVALID_MESH_ID = std::numeric_limits<MeshID>::max();

    constexpr auto INVALID_SHADER_ID = std::numeric_limits<ShaderID>::max();
    constexpr auto INVALID_SHADER_PROGRAM_ID = std::numeric_limits<ShaderProgramID>::max();
    constexpr auto INVALID_MATERIAL_ID = std::numeric_limits<MaterialID>::max();

    constexpr auto INVALID_TEXTURE_ID = std::numeric_limits<TextureID>::max();
}