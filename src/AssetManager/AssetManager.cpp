//
// Created by Oskar.Norberg on 2025-08-27.
//

#include "AssetManager/AssetManager.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

RNGOEngine::AssetHandling::AssetManager::AssetManager(Core::Renderer::IRenderer& renderer)
    : m_renderer(renderer), shaderPreprocessor(m_assetFileFetcher)
{
    AddAssetPath(ENGINE_ASSETS_DIR, All);

    AddAssetPath(ENGINE_SHADERS_DIR, Shader);
    AddAssetPath(ENGINE_SHADER_INCLUDE_DIR, Shader);

    AddAssetPath(ENGINE_MODELS_DIR, Mesh);
    AddAssetPath(ENGINE_TEXTURES_DIR, Texture);
}

RNGOEngine::Core::Renderer::MeshID RNGOEngine::AssetHandling::AssetManager::CreateMesh(
    std::span<float> vertices, std::span<unsigned int> indices)
{
    return m_renderer.CreateMesh(vertices, indices);
}

RNGOEngine::Core::Renderer::MaterialHandle RNGOEngine::AssetHandling::AssetManager::CreateMaterial(
    const std::filesystem::path& vertexSourcePath, const std::filesystem::path& fragmentSourcePath)
{
    const auto processedVertexSource = shaderPreprocessor.Parse(vertexSourcePath);
    const auto processedFragmentSource = shaderPreprocessor.Parse(fragmentSourcePath);

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
    const auto foundPath = m_assetFileFetcher.GetTexturePath(texturePath);

    if (!foundPath.has_value())
    {
        assert(false && "Texture not found.");
        return Core::Renderer::INVALID_TEXTURE_ID;
    }

    int width, height, nrChannels;
    unsigned char* data = stbi_load(foundPath.value().string().data(), &width, &height, &nrChannels, 0);

    if (!data || width <= 0 || height <= 0 || nrChannels <= 0)
    {
        assert(false && "Failed to load texture");
        return Core::Renderer::INVALID_TEXTURE_ID;
    }

    const auto textureHandle = m_renderer.CreateTexture(data, width, height, nrChannels);
    stbi_image_free(data);

    return textureHandle;
}

void RNGOEngine::AssetHandling::AssetManager::AddAssetPath(
    const std::filesystem::path& path, AssetPathType type)
{
    switch (type)
    {
        case All:
            m_assetFileFetcher.AddAssetPath(path);
            break;

        case Shader:
            m_assetFileFetcher.AddShaderPath(path);
            break;

        case Texture:
            m_assetFileFetcher.AddTexturePath(path);
            break;

        case Mesh:
            m_assetFileFetcher.AddMeshPath(path);
            break;

        default:
            assert(false && "Unsupported asset path type");
            break;
    }
}