//
// Created by Oskar.Norberg on 2025-08-26.
//

#pragma once

namespace RNGOEngine::Core::Renderer
{
    using MeshHandle = unsigned int;
    using ShaderHandle = unsigned int;
    using TextureHandle = unsigned int;

    constexpr auto INVALID_MESH_HANDLE = std::numeric_limits<MeshHandle>::max();
    constexpr auto INVALID_SHADER_HANDLE = std::numeric_limits<ShaderHandle>::max();
    constexpr auto INVALID_TEXTURE_HANDLE = std::numeric_limits<TextureHandle>::max();
}