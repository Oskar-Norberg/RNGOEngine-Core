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
        const auto* data = textureHandle.data;
        const auto width = data->width;
        const auto height = data->height;
        const auto nrChannels = data->nrChannels;
        const auto textureData = std::as_bytes(
            std::span<const unsigned char>(data->data, width * height * nrChannels));

        const auto key = m_textures.Insert(m_renderer.CreateTexture(width, height, nrChannels, textureData));
        return key;
    }

    void TextureResourceManager::MarkTextureForDeletion(
        Containers::Vectors::GenerationalKey<Core::Renderer::TextureID> key)
    {
        m_textures.MarkForRemoval(key);
    }

    void TextureResourceManager::DeleteMarkedTextures()
    {
        // TODO:
    }

    std::optional<Core::Renderer::TextureID> TextureResourceManager::GetTexture(
        const Containers::Vectors::GenerationalKey<Core::Renderer::TextureID>& key) const
    {
        return m_textures.GetValidated(key);
    }
}