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

    Containers::GenerationalKey<Core::Renderer::TextureID> TextureResourceManager::CreateTexture(
        const Core::Renderer::Texture2DProperties& properties, const int width, const int height,
        const std::span<const std::byte> textureData)
    {
        const auto textureID = m_renderer.CreateTexture2D(properties, width, height, textureData);
        return m_textures.Insert(textureID);
    }

    void TextureResourceManager::MarkTextureForDeletion(
        const Containers::GenerationalKey<Core::Renderer::TextureID> key)
    {
        m_textures.MarkForRemoval(key);
    }

    std::optional<Core::Renderer::TextureID> TextureResourceManager::GetTexture(
        const Containers::GenerationalKey<Core::Renderer::TextureID>& key) const
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
            const auto textureIDOpt = m_textures.GetUnmarkedValidated(key);
            if (textureIDOpt.has_value())
            {
                m_renderer.DestroyTexture(textureIDOpt->get());
            }
            m_textures.Remove(key);
        }
    }
}