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

    std::expected<std::weak_ptr<TextureAsset>, TextureManagerError> TextureManager::UploadTexture(
        const AssetHandle& assetHandle, const Core::Renderer::Texture2DProperties& properties,
        const int width, const int height, const std::span<const std::byte> textureData
    )
    {
        // Upload Resources
        auto textureKey = m_resourceManager.GetTextureResourceManager().CreateTexture(
            properties, width, height, textureData
        );

        // Store Runtime Data
        auto textureAsset = std::make_shared<TextureAsset>(AssetHandle(assetHandle), std::move(textureKey));
        auto [it, inserted] = m_textures.insert({assetHandle, std::move(textureAsset)});

        return it->second;
    }

    void TextureManager::UnloadTexture(const AssetHandle& assetHandle)
    {
        if (m_textures.contains(assetHandle))
        {
            const auto& runtimeTextureData = m_textures.at(assetHandle);
            const auto& textureKey = runtimeTextureData->GetTextureKey();

            m_resourceManager.GetTextureResourceManager().MarkTextureForDeletion(textureKey);
            m_textures.erase(assetHandle);
        }
    }

    Core::Renderer::TextureID TextureManager::GetTexture(const AssetHandle& uuid) const
    {
        const auto& textureResourceManager = m_resourceManager.GetTextureResourceManager();

        // TODO: Cleanup
        if (!m_textures.contains(uuid))
        {
            // TODO: return opt.
            RNGO_ASSERT(false && "TextureManager::GetTexture - TextureAsset not found.");
        }

        const auto& runtimeTextureData = m_textures.at(uuid);
        const auto textureOpt = textureResourceManager.GetTexture(runtimeTextureData->GetTextureKey());
        if (!textureOpt)
        {
            // TODO: return opt.
            RNGO_ASSERT(false && "TextureManager::GetTexture - TextureAsset not found.");
        }

        return textureOpt.value();
    }
}