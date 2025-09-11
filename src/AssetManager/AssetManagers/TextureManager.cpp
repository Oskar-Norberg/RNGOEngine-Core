//
// Created by Oskar.Norberg on 2025-09-10.
//

#include "AssetManager/AssetManagers/TextureManager.h"

#include "Renderer/IRenderer.h"

namespace RNGOEngine::AssetHandling
{
    TextureManager::TextureManager(Core::Renderer::IRenderer& renderer)
        : m_renderer(renderer)
    {
    }

    Core::Renderer::TextureID TextureManager::GetTexture(const Core::Renderer::TextureID id) const
    {
        return m_textures[id];
    }

    Core::Renderer::TextureID TextureManager::CreateTexture(const TextureLoader::TextureHandle texture)
    {
        m_textures.emplace_back(m_renderer.CreateTexture(texture.data->data, texture.data->width,
                                                         texture.data->height,
                                                         texture.data->nrChannels));

        return m_textures.size() - 1;
    }
}