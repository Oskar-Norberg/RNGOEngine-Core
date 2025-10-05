//
// Created by Oskar.Norberg on 2025-09-10.
//

#pragma once

#include <vector>

#include "Renderer/RenderID.h"
#include "Renderer/Handles/TextureHandle.h"

namespace RNGOEngine
{
    namespace Resources
    {
        class ResourceManager;
    }
}

namespace RNGOEngine::AssetHandling
{
    class TextureManager
    {
    public:
        explicit TextureManager(Resources::ResourceManager& resourceManager);

        Core::Renderer::TextureID GetTexture(Core::Renderer::TextureID id) const;
        Core::Renderer::TextureID CreateTexture(Textures::TextureHandle texture);

    private:
        std::vector<Core::Renderer::TextureID> m_textures;

    private:
        Resources::ResourceManager& m_resourceManager;
    };
}