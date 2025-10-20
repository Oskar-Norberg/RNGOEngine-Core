//
// Created by Oskar.Norberg on 2025-09-10.
//

#include "AssetManager/AssetManagers/TextureManager.h"

#include "AssetManager/AssetLoaders/TextureLoader.h"
#include "Renderer/IRenderer.h"
#include "ResourceManager/ResourceManager.h"

namespace RNGOEngine::AssetHandling
{
    TextureManager::TextureManager(Resources::ResourceManager& resourceManager)
        : m_resourceManager(resourceManager)
    {
    }

    Core::Renderer::TextureID TextureManager::GetTexture(
        Containers::Vectors::GenerationalKey<TextureManagerData> key) const
    {
        if (const auto texture = m_textures.GetValidated(key); texture)
        {
            return texture.value().get().CachedTextureID;
        }

        if (const auto texture = m_textures.GetValidated(GetInvalidTexture()); texture)
        {
            return texture.value().get().CachedTextureID;
        }

        RNGO_ASSERT(false && "TextureManager::GetTexture GetInvalidTexture failed!");
        return Core::Renderer::INVALID_TEXTURE_ID;
    }

    std::expected<Containers::Vectors::GenerationalKey<TextureManagerData>, TextureManagerError>
    TextureManager::CreateTexture(const std::filesystem::path& path)
    {
        // Check Cache
        if (const auto cachedTexture = m_textureCache.TryGet(path); cachedTexture.has_value())
        {
            return cachedTexture.value();
        }

        // Load Texture to RAM
        const auto textureHandle = LoadTexture(path);
        if (!textureHandle.has_value())
        {
            return std::unexpected(textureHandle.error());
        }

        // Upload to GPU & Store in m_textures
        const auto textureKey = m_resourceManager.CreateTexture(textureHandle.value());
        const auto textureIDOpt = m_resourceManager.GetTexture(textureKey);
        if (!textureIDOpt.has_value())
        {
            return std::unexpected(TextureManagerError::FailedToLoad);
        }

        TextureManagerData textureIDData = {
            .TextureKey = textureKey,
            .CachedTextureID = textureIDOpt.value(),
        };
        const auto key = m_textures.Insert(textureIDData);

        // Unload from RAM
        UnloadTexture(textureHandle.value());

        // Cache
        m_textureCache.Insert(path, key);

        return key;
    }

    Containers::Vectors::GenerationalKey<TextureManagerData> TextureManager::GetInvalidTexture() const
    {
        // TODO: Return actual invalid texture
        return m_textures.InvalidKey();
    }

    std::expected<Textures::TextureHandle, TextureManagerError> TextureManager::LoadTexture(
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