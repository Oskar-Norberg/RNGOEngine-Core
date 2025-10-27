//
// Created by Oskar.Norberg on 2025-08-27.
//

#pragma once

#include "AssetHandling/AssetFetcher/AssetFetcher.h"
#include "AssetHandling/AssetManager/Managers/ModelManager.h"
#include "AssetHandling/AssetManager/Managers/MaterialManager.h"
#include "AssetHandling/AssetManager/Managers/ShaderManager.h"
#include "AssetHandling/AssetManager/Managers/TextureManager.h"
#include "Renderer/Handles/MaterialHandle.h"

namespace RNGOEngine::AssetHandling
{
    class AssetDatabase;
}

namespace RNGOEngine
{
    namespace Resources
    {
        class ResourceManager;
    }
}

namespace RNGOEngine::AssetHandling
{
    class AssetManager
    {
    public:
        // TODO: Goal is for AssetManager to not ever use the AssetFetcher. Only interact with assets through the AssetDatabase.
        explicit AssetManager(AssetFetcher& assetFetcher, AssetDatabase& assetDatabase, Resources::ResourceManager& resourceManager, bool doFlipUVs);

    public:
        AssetHandle LoadModel(const std::filesystem::path& modelPath);

    public:
        Core::Renderer::MaterialHandle CreateMaterial(const std::filesystem::path& vertexSourcePath,
                                                      const std::filesystem::path& fragmentSourcePath);

        Containers::Vectors::GenerationalKey<TextureManagerData> LoadTexture(std::string_view texturePath);

    public:
        const ShaderManager& GetShaderManager() const
        {
            return m_shaderManager;
        }
        
        const MaterialManager& GetMaterialManager() const
        {
            return m_materialManager;
        }

        const ModelManager& GetModelManager() const
        {
            return m_modelManager;
        }

        const TextureManager& GetTextureManager() const
        {
            return m_textureManager;
        }

        // Engine Internal
    public:
        void RebuildResourceCaches();

    private:
        AssetDatabase& m_assetDatabase;
        // TODO: Manager should get from the Database in the future. Remove this when possible.
        AssetFetcher& m_assetFileFetcher;
        
        ShaderManager m_shaderManager;
        ModelManager m_modelManager;
        TextureManager m_textureManager;
        MaterialManager m_materialManager;
    };
}