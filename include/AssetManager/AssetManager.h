//
// Created by Oskar.Norberg on 2025-08-27.
//

#pragma once

#include "AssetManager/AssetManagers/ModelManager.h"

#include "AssetLoaders/MaterialLoader.h"
#include "AssetLoaders/ModelLoader.h"
#include "AssetLoaders/ShaderLoader.h"
#include "AssetLoaders/TextureLoader.h"
#include "AssetManagers/TextureManager.h"
#include "Renderer/Handles/MaterialHandle.h"

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

        // TODO: This is internal API to the RenderSystem. This should probably not be public.
        std::optional<std::reference_wrapper<const ModelData>> GetModel(ModelID id);

    public:

        Core::Renderer::MaterialHandle CreateMaterial(const std::filesystem::path& vertexSourcePath,
                                                      const std::filesystem::path& fragmentSourcePath);

        Core::Renderer::TextureID LoadTexture(std::string_view texturePath);

    public:
        void AddAssetPath(const std::filesystem::path& path, AssetPathType type);

    private:
        Core::Renderer::IRenderer& m_renderer;
        AssetFileFetcher m_assetFileFetcher;

        ModelManager m_modelManager;
        TextureManager m_textureManager;
        
        ModelLoader m_modelLoader;
        ShaderLoader m_shaderLoader;
        MaterialLoader m_materialLoader;
    };
}