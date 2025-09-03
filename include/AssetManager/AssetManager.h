//
// Created by Oskar.Norberg on 2025-08-27.
//

#pragma once

#include "AssetLoaders/MaterialLoader.h"
#include "AssetLoaders/ShaderLoader.h"
#include "AssetLoaders/TextureLoader.h"
#include "Renderer/IRenderer.h"
#include "Renderer/Handles/MaterialHandle.h"
#include "Shader/ShaderPreprocessor.h"

namespace RNGOEngine::AssetHandling
{
    enum AssetPathType { All, Shader, Texture, Mesh };

    class AssetManager
    {
    public:
        explicit AssetManager(Core::Renderer::IRenderer& renderer, bool doFlipTexturesVertically);

        Core::Renderer::MeshID CreateMesh(std::span<float> vertices, std::span<unsigned> indices);

        Core::Renderer::MaterialHandle CreateMaterial(const std::filesystem::path& vertexSourcePath,
                                                      const std::filesystem::path& fragmentSourcePath);

        Core::Renderer::TextureID LoadTexture(std::string_view texturePath);

    public:
        void AddAssetPath(const std::filesystem::path& path, AssetPathType type);

    private:
        Core::Renderer::IRenderer& m_renderer;
        AssetFileFetcher m_assetFileFetcher;
        
        TextureLoader m_textureLoader;
        
        ShaderLoader m_shaderLoader;
        MaterialLoader m_materialLoader;
    };
}