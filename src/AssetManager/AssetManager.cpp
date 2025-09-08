//
// Created by Oskar.Norberg on 2025-08-27.
//

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "AssetManager/AssetManager.h"

#include "Renderer/IRenderer.h"

RNGOEngine::AssetHandling::AssetManager::AssetManager(Core::Renderer::IRenderer& renderer,
                                                      bool doFlipTexturesVertically)
    : m_renderer(renderer),
      m_textureLoader(renderer, m_assetFileFetcher, doFlipTexturesVertically),
      m_shaderLoader(renderer, m_assetFileFetcher),
      m_materialLoader(renderer)
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
    Assimp::Importer importer;
    return m_renderer.CreateMesh(vertices, indices);
}

RNGOEngine::Core::Renderer::MaterialHandle RNGOEngine::AssetHandling::AssetManager::CreateMaterial(
    const std::filesystem::path& vertexSourcePath, const std::filesystem::path& fragmentSourcePath)
{
    // TODO: Caching
    const auto vertexShaderID = m_shaderLoader.LoadShader(vertexSourcePath, Core::Renderer::Vertex);
    const auto fragmentShaderID = m_shaderLoader.LoadShader(fragmentSourcePath, Core::Renderer::Fragment);

    const auto programID = m_shaderLoader.CreateShaderProgram(vertexShaderID, fragmentShaderID);
    const auto materialID = m_renderer.CreateMaterial(programID);

    return Core::Renderer::MaterialHandle(materialID, m_renderer);
}

RNGOEngine::Core::Renderer::TextureID RNGOEngine::AssetHandling::AssetManager::LoadTexture(
    std::string_view texturePath)
{
    // TODO: Caching

    return m_textureLoader.LoadTexture(texturePath);
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