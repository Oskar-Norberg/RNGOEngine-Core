//
// Created by Oskar.Norberg on 2025-09-10.
//

#pragma once

#include <expected>

#include "Assets/AssetDatabase/AssetDatabase.h"
#include "Renderer/RenderID.h"
#include "Utilities/AssetCache/AssetCache.h"

namespace RNGOEngine
{
    namespace Resources
    {
        class ResourceManager;
    }
}

namespace RNGOEngine::AssetHandling
{
    enum class TextureManagerError
    {
        None,
        // TODO:
    };

    struct RuntimeTextureData
    {
    };

    class TextureManager
    {
    public:
        explicit TextureManager(Resources::ResourceManager& resourceManager);

    public:
        std::expected<std::weak_ptr<TextureAsset>, TextureManagerError> UploadTexture(const AssetHandle& assetHandle,
                                          const Core::Renderer::Texture2DProperties& properties,
                                          int width, int height,
                                          std::span<const std::byte> textureData);
        void UnloadTexture(const AssetHandle& assetHandle);

    public:
        Core::Renderer::TextureID GetTexture(const AssetHandle& uuid) const;

    private:
        std::unordered_map<AssetHandle, std::shared_ptr<TextureAsset>> m_textures;

    private:
        Resources::ResourceManager& m_resourceManager;
    };
}