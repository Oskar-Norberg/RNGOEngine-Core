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
        const Core::Renderer::TextureID id) const
    {
        const auto textureOpt = m_resourceManager.GetTexture(m_textures[id]);
        if (textureOpt)
        {
            return textureOpt.value();
        }

        RNGO_ASSERT(false && "TextureManager::GetTexture: Invalid texture key.");
        // TODO: Return a default/error texture?
        return Core::Renderer::INVALID_TEXTURE_ID;
    }

    std::expected<Core::Renderer::TextureID, TextureManagerError> TextureManager::CreateTexture(
        const std::filesystem::path& path)
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
        const auto textureID = m_resourceManager.CreateTexture(textureHandle.value());
        const auto id = m_textures.size();
        m_textures.emplace_back(textureID);

        // Unload from RAM
        UnloadTexture(textureHandle.value());

        // Cache
        m_textureCache.Insert(path, id);

        return m_textures.size() - 1;
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