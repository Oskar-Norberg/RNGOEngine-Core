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

    TextureManagerError TextureManager::UploadTexture(const AssetHandle& assetHandle,
                                                      const Core::Renderer::Texture2DProperties& properties,
                                                      const int width, const int height,
                                                      const std::span<const std::byte> textureData)
    {
        // Upload Resources
        const auto textureKey = m_resourceManager.GetTextureResourceManager().CreateTexture(
            properties, width, height, textureData);

        // Store Runtime Data
        m_textures.insert({assetHandle, {textureKey}});

        // TODO:
        return TextureManagerError::None;
    }

    void TextureManager::UnloadTexture(const AssetHandle& assetHandle)
    {
        if (m_textures.contains(assetHandle))
        {
            const auto& runtimeTextureData = m_textures.at(assetHandle);
            const auto& textureKey = runtimeTextureData.TextureKey;

            m_resourceManager.GetTextureResourceManager().MarkTextureForDeletion(textureKey);
            m_textures.erase(assetHandle);
        }
    }

    AssetHandle TextureManager::GetInvalidTexture() const
    {
        return Utilities::UUID(0);
    }

    Core::Renderer::TextureID TextureManager::GetTexture(const AssetHandle& uuid) const
    {
        if (!m_textures.contains(uuid))
        {
            RNGO_ASSERT(false && "TextureManager::GetTexture called with invalid UUID.");
            // TODO: Return actual error texture.
            return Core::Renderer::TextureID();
        }

        const auto& runtimeTextureData = m_textures.at(uuid);
        const auto& textureResourceManager = m_resourceManager.GetTextureResourceManager();
        const auto textureOpt = textureResourceManager.GetTexture(runtimeTextureData.TextureKey);
        if (!textureOpt)
        {
            RNGO_ASSERT(false && "TextureManager::GetTexture Texture has been invalidated.");
            return Core::Renderer::TextureID();
        }

        return textureOpt.value();
    }
}