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

    Core::Renderer::TextureID TextureManager::CreateTexture(TextureLoader::TextureHandle texture)
    {
        return m_renderer.CreateTexture(texture.data->data, texture.data->width, texture.data->height,
                                 texture.data->nrChannels);
    }
}