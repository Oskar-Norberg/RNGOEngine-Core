//
// Created by Oskar.Norberg on 2025-08-27.
//

#pragma once

#include "AssetManager/AssetManagers/ModelManager.h"

#include "AssetLoaders/ModelLoader.h"
#include "AssetLoaders/ShaderLoader.h"
#include "AssetLoaders/TextureLoader.h"
#include "AssetManagers/MaterialManager.h"
#include "AssetManagers/ShaderManager.h"
#include "AssetManagers/TextureManager.h"
#include "Renderer/Handles/MaterialHandle.h"
#include "Utilities/AssetCache/AssetCache.h"

namespace RNGOEngine
{
    namespace Core
    {
        namespace Renderer
        {
            class IRenderer;
        }
    }
}

namespace RNGOEngine::AssetHandling
{
    enum AssetPathType { All, Shader, Texture, Mesh };

    class AssetManager
    {
    public:
        explicit AssetManager(Core::Renderer::IRenderer& renderer, bool doFlipTexturesVertically);

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
        // TODO: This is internal API to the RenderSystem. This should probably not be public.
        std::optional<std::reference_wrapper<const ModelData>> GetModel(ModelID id) const;
        std::optional<Core::Renderer::TextureID> GetTexture(Core::Renderer::TextureID id) const;

    public:
        const MaterialManager& GetMaterialManager() const { return m_materialManager; }
        const ModelManager& GetModelManager() const { return m_modelManager; }
        const TextureManager& GetTextureManager() const { return m_textureManager; }
        
    private:
        bool m_doFlipTexturesVertically;
        
    private:
        Core::Renderer::IRenderer& m_renderer;
        AssetFileFetcher m_assetFileFetcher;

        ShaderLoader m_shaderLoader;
        ShaderManager m_shaderManager;

        Utilities::AssetCache<std::filesystem::path, ModelID> m_modelCache;
        ModelManager m_modelManager;
        
        TextureManager m_textureManager;
        MaterialManager m_materialManager;

    };
}