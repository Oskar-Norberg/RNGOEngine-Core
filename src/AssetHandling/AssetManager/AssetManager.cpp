//
// Created by Oskar.Norberg on 2025-08-27.
//

#include "AssetHandling/AssetManager/AssetManager.h"

#include "Renderer/IRenderer.h"
#include "Utilities/RNGOAsserts.h"

namespace RNGOEngine::AssetHandling
{
    AssetManager::AssetManager(AssetFetcher& assetFetcher, Resources::ResourceManager& resourceManager,
                               const bool doFlipUVs)
        : m_assetFileFetcher(assetFetcher),
          m_shaderManager(resourceManager, m_assetFileFetcher),
          m_modelManager(resourceManager, doFlipUVs),
          m_textureManager(resourceManager)
    {

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

        const auto materialKey = m_materialManager.CreateMaterial(shaderProgram.value());
        return Core::Renderer::MaterialHandle(materialKey, m_materialManager);
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

    

    void AssetManager::RebuildResourceCaches()
    {
        m_modelManager.RebuildCache();
        m_textureManager.RebuildCache();
        m_shaderManager.RebuildCache();
    }
}