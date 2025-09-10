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

    Core::Renderer::TextureID TextureManager::GetTexture(Core::Renderer::TextureID id) const
    {
        return m_textures[id];
    }

    std::optional<Core::Renderer::TextureID> TextureManager::GetTextureIfLoaded(
        const std::filesystem::path& path) const
    {
        if (const auto it = m_loadedTexturePaths.find(path); it != m_loadedTexturePaths.end())
        {
            return it->second;
        }

        return std::nullopt;
    }

    Core::Renderer::TextureID TextureManager::CreateTexture(const TextureLoader::TextureHandle texture)
    {
        m_textures.emplace_back(m_renderer.CreateTexture(texture.data->data, texture.data->width,
                                                         texture.data->height,
                                                         texture.data->nrChannels));

        const size_t prevSize = m_textures.size() - 1;
        m_loadedTexturePaths[texture.data->path] = prevSize;
        return m_textures[prevSize];
    }
}