//
// Created by Oskar.Norberg on 2025-09-10.
//

#include "Assets/AssetManager/Managers/TextureManager.h"

#include "Assets/AssetLoaders/TextureLoader.h"
#include "ResourceManager/ResourceManager.h"

namespace RNGOEngine::AssetHandling
{
    TextureManager::TextureManager(AssetDatabase& assetDatabase, Resources::ResourceManager& resourceManager)
        : m_resourceManager(resourceManager),
          m_assetDatabase(assetDatabase)
    {
    }

    std::expected<AssetHandle, TextureManagerError> TextureManager::CreateTexture(
        const std::filesystem::path& path)
    {
        // Check database
        if (const auto textureUUID = m_assetDatabase.TryGetTextureUUID(path); textureUUID)
        {
            return textureUUID.value();
        }

        // Load Texture to RAM
        const auto textureHandle = LoadFromDisk(path);
        if (!textureHandle.has_value())
        {
            return std::unexpected(textureHandle.error());
        }

        // Upload to GPU
        const auto textureKey = m_resourceManager.CreateTexture(textureHandle.value());

        // Unload from RAM
        UnloadTexture(textureHandle.value());

        // Upload to Database
        RuntimeTextureData textureIDData = {
            .TextureKey = textureKey
        };

        auto uuid = m_assetDatabase.Insert(textureHandle.value(), path);
        m_textures.insert({uuid, textureIDData});

        // Mark Asset as consumed
        m_assetDatabase.SetAssetState(uuid, AssetState::Consumed);

        return uuid;
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

    void TextureManager::BeginDestroyAllTextures()
    {
        for (const auto& [handle, textureData] : m_textures)
        {
            m_resourceManager.MarkTextureForDestruction(textureData.TextureKey);
        }
    }

    std::expected<Textures::TextureHandle, TextureManagerError> TextureManager::LoadFromDisk(
        const std::filesystem::path& path)
    {
        const auto textureHandle = TextureLoader::LoadTexture(path);

        if (!textureHandle.has_value())
        {
            switch (textureHandle.error())
            {
                case TextureLoader::TextureLoadingError::None:
                    return std::unexpected(TextureManagerError::None);
                case TextureLoader::TextureLoadingError::FileNotFound:
                    return std::unexpected(TextureManagerError::FileNotFound);
                case TextureLoader::TextureLoadingError::FailedToLoad:
                    return std::unexpected(TextureManagerError::FailedToLoad);
                default:
                    return std::unexpected(TextureManagerError::FailedToLoad);
            }
        }

        return textureHandle.value();
    }

    void TextureManager::UnloadTexture(Textures::TextureHandle handle)
    {
        TextureLoader::FreeTexture(handle);
    }
}