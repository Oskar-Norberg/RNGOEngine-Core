//
// Created by Oskar.Norberg on 2025-08-27.
//

#pragma once

#include "Assets/AssetFetcher/AssetFetcher.h"
#include "Assets/AssetManager/Managers/ModelManager.h"
#include "Assets/AssetManager/Managers/MaterialManager.h"
#include "Assets/AssetManager/Managers/ShaderManager.h"
#include "Assets/AssetManager/Managers/TextureManager.h"
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
    // TODO: Refactor this into a service-locator like-pattern.
    class AssetManager
    {
    public:
        explicit AssetManager(AssetFetcher& assetFetcher, AssetDatabase& assetDatabase,
                              Resources::ResourceManager& resourceManager, bool doFlipUVs);

    // public:
    //     AssetHandle LoadModel(const std::filesystem::path& modelPath);

    public:
        Core::Renderer::MaterialHandle CreateMaterial(const std::filesystem::path& vertexSourcePath,
                                                      const std::filesystem::path& fragmentSourcePath);

        AssetHandle LoadTexture(std::string_view texturePath);

        // Non-const Getters
    public:
        ShaderManager& GetShaderManager()
        {
            return m_shaderManager;
        }

        MaterialManager& GetMaterialManager()
        {
            return m_materialManager;
        }

        ModelManager& GetModelManager()
        {
            return m_modelManager;
        }

        TextureManager& GetTextureManager()
        {
            return m_textureManager;
        }

        // Const Getters
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
        void BeginDestroyAllAssets();

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