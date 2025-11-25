//
// Created by Oskar.Norberg on 2025-09-10.
//

#include "Assets/AssetManager/Managers/TextureManager.h"

#include "Assets/AssetLoaders/TextureLoader.h"
#include "ResourceManager/ResourceManager.h"

namespace RNGOEngine::AssetHandling
{
    TextureManager::TextureManager(Resources::ResourceManager& resourceManager)
        : m_resourceManager(resourceManager)
    {
    }

    std::expected<TextureAsset, TextureManagerError> TextureManager::UploadTexture(
        const AssetHandle& assetHandle, const Core::Renderer::Texture2DProperties& properties,
        const int width, const int height, const std::span<const std::byte> textureData
    )
    {
        // Upload Resources
        auto textureKey = m_resourceManager.GetTextureResourceManager().CreateTexture(
            properties, width, height, textureData
        );

        // Store Runtime Data
        auto textureAsset = TextureAsset(AssetHandle(assetHandle), std::move(textureKey));

        return textureAsset;
    }

    void TextureManager::UnloadTexture(const AssetHandle& assetHandle)
    {
        // TODO:
    }
}