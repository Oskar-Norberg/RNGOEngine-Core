//
// Created by Oskar.Norberg on 2025-09-10.
//

#include "AssetManager/AssetManagers/TextureManager.h"

#include "Renderer/IRenderer.h"
#include "ResourceManager/ResourceManager.h"

namespace RNGOEngine::AssetHandling
{
    TextureManager::TextureManager(Resources::ResourceManager& resourceManager)
        : m_resourceManager(resourceManager)
    {
    }

    Core::Renderer::TextureID TextureManager::GetTexture(const Core::Renderer::TextureID id) const
    {
        return m_textures[id];
    }

    Core::Renderer::TextureID TextureManager::CreateTexture(const Textures::TextureHandle texture)
    {
        m_textures.emplace_back(m_resourceManager.CreateTexture(texture));

        return m_textures.size() - 1;
    }
}