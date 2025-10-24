//
// Created by Oskar.Norberg on 2025-09-10.
//

#include "AssetHandling/AssetManager/Managers/TextureManager.h"

#include "AssetHandling/AssetLoaders/TextureLoader.h"
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
        if (const auto texture = m_textures.GetUnmarkedValidated(key); texture)
        {
            return texture.value().get().CachedTextureID;
        }

        if (const auto texture = m_textures.GetUnmarkedValidated(GetInvalidTexture()); texture)
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
        if (const auto cachedTexture = m_textureCache.TryGet(path); cachedTexture)
        {
            if (m_textures.GetUnmarkedValidated(cachedTexture.value()))
            {
                return cachedTexture.value();
            }
            else
            {
                m_textureCache.Remove(path);
            }
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
            .TextureKey = textureKey
        };
        const auto key = m_textures.Insert(std::move(textureIDData));
        UpdateTextureCache(key);

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

    Containers::Vectors::GenerationalKey<Core::Renderer::TextureID> TextureManager::GetTextureKey(
        const Containers::Vectors::GenerationalKey<TextureManagerData>& key) const
    {
        const auto textureDataOpt = m_textures.GetUnmarkedValidated(key);
        if (!textureDataOpt)
        {
            RNGO_ASSERT(false && "TextureManager::GetTextureKey called with invalid texture key.");
            return Containers::Vectors::GenerationalVector<Core::Renderer::TextureID>::InvalidKey();
        }

        return textureDataOpt.value().get().TextureKey;
    }

    void TextureManager::RebuildCache()
    {
        for (const auto& key : m_textures.Live())
        {
            UpdateTextureCache(key);
        }
    }

    void TextureManager::UpdateTextureCache(
        const Containers::Vectors::GenerationalKey<TextureManagerData>& key)
    {
        // TODO: This both updates cache and invalidates model if texture is missing. Separate.
        const auto textureDataOpt = m_textures.GetUnmarkedValidated(key);
        if (!textureDataOpt)
        {
            RNGO_ASSERT(false && "TextureManager::UpdateTextureCache called with invalid texture key.");
            return;
        }

        auto& [textureKey, cachedID] = textureDataOpt.value().get();
        bool invalid = false;

        const auto textureIDOpt = m_resourceManager.GetTexture(textureKey);
        if (textureIDOpt.has_value())
        {
            cachedID = textureIDOpt.value();
        }
        else
        {
            invalid = true;
        }

        if (invalid)
        {
            m_textures.Remove(key);
        }
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