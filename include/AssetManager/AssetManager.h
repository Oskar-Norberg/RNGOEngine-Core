//
// Created by Oskar.Norberg on 2025-08-27.
//

#pragma once

#include "AssetManager/AssetManagers/ModelManager.h"
#include "AssetManagers/MaterialManager.h"
#include "AssetManagers/ShaderManager.h"
#include "AssetManagers/TextureManager.h"
#include "Renderer/Handles/MaterialHandle.h"

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
        explicit AssetManager(Resources::ResourceManager& resourceManager, bool doFlipUVs);

    public:
        ModelID LoadModel(const std::filesystem::path& modelPath);

    public:
        Core::Renderer::MaterialHandle CreateMaterial(const std::filesystem::path& vertexSourcePath,
                                                      const std::filesystem::path& fragmentSourcePath);

        Core::Renderer::TextureID LoadTexture(std::string_view texturePath);

    public:
        void AddAssetPath(const std::filesystem::path& path, AssetPathType type);

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

    private:
        AssetFileFetcher m_assetFileFetcher;
        
        ShaderManager m_shaderManager;
        ModelManager m_modelManager;
        TextureManager m_textureManager;
        MaterialManager m_materialManager;
    };
}