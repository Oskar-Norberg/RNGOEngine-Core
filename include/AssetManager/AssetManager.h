//
// Created by Oskar.Norberg on 2025-08-27.
//

#pragma once

#include "AssetManager/AssetManagers/ModelManager.h"

#include "AssetLoaders/ShaderLoader.h"
#include "AssetManagers/MaterialManager.h"
#include "AssetManagers/ShaderManager.h"
#include "AssetManagers/TextureManager.h"
#include "Renderer/Handles/MaterialHandle.h"
#include "Utilities/AssetCache/AssetCache.h"
#include "Utilities/Hash/PairHash.h"

namespace RNGOEngine
{
    namespace Resources
    {
        class ResourceManager;
    }
}

namespace RNGOEngine::AssetHandling
{
    enum AssetPathType { All, Shader, Texture, Mesh };

    class AssetManager
    {
    public:
        explicit AssetManager(Resources::ResourceManager& resourceManager, bool doFlipTexturesVertically);

        // Models
    public:
        ModelID LoadModel(const std::filesystem::path& modelPath);

    public:
        Core::Renderer::MaterialHandle CreateMaterial(const std::filesystem::path& vertexSourcePath,
                                                      const std::filesystem::path& fragmentSourcePath);

        Core::Renderer::TextureID LoadTexture(std::string_view texturePath);

    public:
        void AddAssetPath(const std::filesystem::path& path, AssetPathType type);

    public:
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

    private:
        bool m_doFlipTexturesVertically;

    private:
        AssetFileFetcher m_assetFileFetcher;

        Utilities::AssetCache<std::filesystem::path, Core::Renderer::ShaderID> m_shaderCache;
        Utilities::AssetCache<std::pair<std::filesystem::path, std::filesystem::path>,
                              Core::Renderer::ShaderID, Utilities::Hash::PairHash> m_shaderProgramCache;
        ShaderLoader m_shaderLoader;
        ShaderManager m_shaderManager;

        Utilities::AssetCache<std::filesystem::path, ModelID> m_modelCache;
        ModelManager m_modelManager;

        Utilities::AssetCache<std::filesystem::path, ModelID> m_textureCache;
        TextureManager m_textureManager;
        MaterialManager m_materialManager;
    };
}