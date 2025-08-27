//
// Created by Oskar.Norberg on 2025-08-27.
//

#include "AssetManager/AssetManager.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

RNGOEngine::AssetHandling::AssetManager::AssetManager(Core::Renderer::IRenderer& renderer)
    : m_renderer(renderer)
{
}

RNGOEngine::Core::Renderer::MeshHandle RNGOEngine::AssetHandling::AssetManager::CreateMesh(
    std::span<float> vertices, std::span<unsigned int> indices) const
{
    return m_renderer.CreateMesh(vertices, indices);
}

RNGOEngine::Core::Renderer::ShaderHandle RNGOEngine::AssetHandling::AssetManager::CreateShader(
    std::string_view vertexSource, std::string_view fragmentSource) const
{
    return m_renderer.CreateShader(vertexSource, fragmentSource);
}

RNGOEngine::Core::Renderer::TextureHandle RNGOEngine::AssetHandling::AssetManager::CreateTexture(
    std::string_view texturePath) const
{
    int width, height, nrChannels;
    unsigned char* data = stbi_load(texturePath.data(), &width, &height, &nrChannels, 0);

    if (!data || width <= 0 || height <= 0 || nrChannels <= 0)
    {
        return Core::Renderer::INVALID_TEXTURE_HANDLE;
    }

    auto textureHandle = m_renderer.CreateTexture(data, width, height, nrChannels);
    stbi_image_free(data);

    return textureHandle;
}