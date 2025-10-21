//
// Created by Oskar.Norberg on 2025-08-27.
//

#include "AssetManager/AssetManager.h"

#include "Renderer/IRenderer.h"
#include "Utilities/RNGOAsserts.h"

namespace RNGOEngine::AssetHandling
{
    AssetManager::AssetManager(Resources::ResourceManager& resourceManager, const bool doFlipUVs)
        : m_shaderManager(resourceManager, m_assetFileFetcher),
          m_modelManager(resourceManager, doFlipUVs),
          m_textureManager(resourceManager)
    {
        AddAssetPath(ENGINE_ASSETS_DIR, All);

        AddAssetPath(ENGINE_SHADERS_DIR, Shader);
        AddAssetPath(ENGINE_SHADER_INCLUDE_DIR, Shader);

        AddAssetPath(ENGINE_MODELS_DIR, Mesh);
        AddAssetPath(ENGINE_TEXTURES_DIR, Texture);
    }


    Containers::Vectors::GenerationalKey<ModelData> AssetManager::LoadModel(
        const std::filesystem::path& modelPath)
    {
        const auto fullPath = m_assetFileFetcher.GetMeshPath(modelPath);
        if (!fullPath.has_value())
        {
            RNGO_ASSERT(false && "Model not found!");
            return m_modelManager.GetInvalidModel();
        }

        const auto model = m_modelManager.CreateModel(fullPath.value());
        if (!model)
        {
            RNGO_ASSERT(false && "Model creation failed!");
            return m_modelManager.GetInvalidModel();
        }

        return model.value();
    }

    Core::Renderer::MaterialHandle AssetManager::CreateMaterial(
        const std::filesystem::path& vertexSourcePath, const std::filesystem::path& fragmentSourcePath)
    {
        const auto shaderProgram = m_shaderManager.LoadShaderProgram(vertexSourcePath, fragmentSourcePath);
        if (!shaderProgram.has_value())
        {
            RNGO_ASSERT(false && "Shader program could not compile!");
        }

        const auto materialID = m_materialManager.CreateMaterial(shaderProgram.value());
        return Core::Renderer::MaterialHandle(materialID, m_materialManager);
    }

    Containers::Vectors::GenerationalKey<TextureManagerData> AssetManager::LoadTexture(
        const std::string_view texturePath)
    {
        const auto fullPath = m_assetFileFetcher.GetTexturePath(texturePath);
        if (!fullPath.has_value())
        {
            RNGO_ASSERT(false && "Texture not found!");
            return m_textureManager.GetInvalidTexture();
        }

        const auto textureID = m_textureManager.CreateTexture(fullPath.value());
        if (!textureID.has_value())
        {
            RNGO_ASSERT(false && "Texture creation failed!");
            return m_textureManager.GetInvalidTexture();
        }

        return textureID.value();
    }

    void AssetManager::AddAssetPath(
        const std::filesystem::path& path, const AssetPathType type)
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
                RNGO_ASSERT(false && "Unsupported asset path type");
                break;
        }
    }

    void AssetManager::OnMeshDestroyed(
        const Containers::Vectors::GenerationalKey<Resources::MeshResource>& meshKey)
    {
        m_modelManager.OnMeshDestroyed(meshKey);
    }

    void AssetManager::RebuildResourceCaches()
    {
        m_modelManager.RebuildCache();
    }
}