//
// Created by Oskar.Norberg on 2025-10-14.
//

#include "ResourceManager/TextureResourceManager/TextureResourceManager.h"

#include "Renderer/IRenderer.h"

namespace RNGOEngine::Resources
{
    TextureResourceManager::TextureResourceManager(RNGOEngine::Core::Renderer::IRenderer& renderer)
        : m_renderer(renderer)
    {
    }

    Containers::Vectors::GenerationalKey<Core::Renderer::TextureID> TextureResourceManager::CreateTexture(
        const AssetHandling::Textures::TextureHandle textureHandle)
    {
        const auto textureID = UploadTexture(textureHandle);
        const auto key = m_textures.Insert(textureID);

        return key;
    }

    void TextureResourceManager::MarkTextureForDeletion(
        Containers::Vectors::GenerationalKey<Core::Renderer::TextureID> key)
    {
        m_textures.MarkForRemoval(key);
    }

    Core::Renderer::TextureID TextureResourceManager::UploadTexture(
        const AssetHandling::Textures::TextureHandle textureHandle)
    {
        const auto* data = textureHandle.data;
        const auto width = data->width;
        const auto height = data->height;
        const auto nrChannels = data->nrChannels;
        const auto textureData = std::as_bytes(
            std::span<const unsigned char>(data->data, width * height * nrChannels));

        return m_renderer.CreateTexture(width, height, nrChannels, textureData);
    }

    void TextureResourceManager::DestroyTexture(const Core::Renderer::TextureID texture)
    {
        m_renderer.DestroyTexture(texture);
    }

    std::optional<Core::Renderer::TextureID> TextureResourceManager::GetTexture(
        const Containers::Vectors::GenerationalKey<Core::Renderer::TextureID>& key) const
    {
        return m_textures.GetUnmarkedValidated(key);
    }

    void TextureResourceManager::MarkAllTextures()
    {
        for (const auto& key : m_textures.Live())
        {
            m_textures.MarkForRemoval(key);
        }
    }

    void TextureResourceManager::DestroyMarkedTextures()
    {
        for (const auto key : m_textures.Marked())
        {
            DestroyTexture(m_textures.GetMarked(key));
            m_textures.Remove(key);
        }
    }
}