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

    TextureManagerError TextureManager::UploadTexture(
        const AssetHandle& assetHandle, const Textures::TextureHandle textureHandle
    )
    {
        // Upload Resources
        const auto textureKey = m_resourceManager.CreateTexture(textureHandle);

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
            m_resourceManager.MarkTextureForDestruction(runtimeTextureData.TextureKey);
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
        const auto textureOpt = m_resourceManager.GetTexture(runtimeTextureData.TextureKey);
        if (!textureOpt)
        {
            RNGO_ASSERT(false && "TextureManager::GetTexture Texture has been invalidated.");
            return Core::Renderer::TextureID();
        }

        return textureOpt.value();
    }
}