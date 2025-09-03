//
// Created by Oskar.Norberg on 2025-08-27.
//

#include "AssetManager/AssetManager.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>


RNGOEngine::AssetHandling::AssetManager::AssetManager(Core::Renderer::IRenderer& renderer)
    : m_renderer(renderer)
{
    shaderPreprocessor.AddDefinition("NR_OF_POINTLIGHTS", std::to_string(Core::Renderer::NR_OF_POINTLIGHTS));
    shaderPreprocessor.AddDefinition("NR_OF_SPOTLIGHTS", std::to_string(Core::Renderer::NR_OF_SPOTLIGHTS));

}

RNGOEngine::Core::Renderer::MeshID RNGOEngine::AssetHandling::AssetManager::CreateMesh(
    std::span<float> vertices, std::span<unsigned int> indices)
{
    return m_renderer.CreateMesh(vertices, indices);
}

RNGOEngine::Core::Renderer::MaterialHandle RNGOEngine::AssetHandling::AssetManager::CreateMaterial(
    std::string_view vertexSource, std::string_view fragmentSource)
{
    const auto processedVertexSource = shaderPreprocessor.Parse(vertexSource);
    const auto processedFragmentSource = shaderPreprocessor.Parse(fragmentSource);
    
    const auto shaderID = m_renderer.CreateShader(processedVertexSource, processedFragmentSource);
    
    if (shaderID == Core::Renderer::INVALID_SHADER_ID)
    {
        return Core::Renderer::MaterialHandle(Core::Renderer::INVALID_MATERIAL_ID, m_renderer);
    }

    const auto materialID = m_renderer.CreateMaterial(shaderID);
    return Core::Renderer::MaterialHandle(materialID, m_renderer);
}

RNGOEngine::Core::Renderer::TextureID RNGOEngine::AssetHandling::AssetManager::CreateTexture(
    std::string_view texturePath)
{
    int width, height, nrChannels;
    unsigned char* data = stbi_load(texturePath.data(), &width, &height, &nrChannels, 0);

    if (!data || width <= 0 || height <= 0 || nrChannels <= 0)
    {
        assert(false && "Failed to load texture");
        return Core::Renderer::INVALID_TEXTURE_ID;
    }

    auto textureHandle = m_renderer.CreateTexture(data, width, height, nrChannels);
    stbi_image_free(data);

    return textureHandle;
}