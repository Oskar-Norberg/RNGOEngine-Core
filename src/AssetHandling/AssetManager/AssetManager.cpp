//
// Created by Oskar.Norberg on 2025-08-27.
//

#include "AssetHandling/AssetManager/AssetManager.h"

#include "Renderer/IRenderer.h"
#include "Utilities/RNGOAsserts.h"

namespace RNGOEngine::AssetHandling
{
    AssetManager::AssetManager(AssetFetcher& assetFetcher, AssetDatabase& assetDatabase,
                               Resources::ResourceManager& resourceManager, bool doFlipUVs)
        : m_assetDatabase(assetDatabase),
          m_assetFileFetcher(assetFetcher),
          m_shaderManager(assetDatabase, resourceManager, m_assetFileFetcher),
          m_modelManager(assetDatabase, resourceManager, doFlipUVs),
          m_textureManager(assetDatabase, resourceManager),
          m_materialManager(assetDatabase, m_shaderManager, m_textureManager)
    {
    }

    AssetHandle AssetManager::LoadModel(
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
        const auto vertexShader = m_shaderManager.CreateShader(vertexSourcePath,
                                                               Core::Renderer::ShaderType::Vertex);
        const auto fragmentShader = m_shaderManager.CreateShader(fragmentSourcePath,
                                                                 Core::Renderer::ShaderType::Fragment);

        const auto materialKey = m_materialManager.CreateMaterial(vertexShader, fragmentShader);

        return Core::Renderer::MaterialHandle(materialKey, m_materialManager);
    }

    AssetHandle AssetManager::LoadTexture(
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


    void AssetManager::RebuildResourceCaches()
    {
        // m_modelManager.RebuildCache();
        // m_textureManager.RebuildCache();
        // m_shaderManager.RebuildCache();
    }
}