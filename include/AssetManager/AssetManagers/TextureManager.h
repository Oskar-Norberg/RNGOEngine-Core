//
// Created by Oskar.Norberg on 2025-09-10.
//

#pragma once

#include <vector>

#include "AssetManager/AssetLoaders/TextureLoader.h"
#include "Renderer/RenderID.h"

namespace RNGOEngine
{
    namespace Core
    {
        namespace Renderer
        {
            class IRenderer;
        }
    }
}

namespace RNGOEngine::AssetHandling
{
    class TextureManager
    {
    public:
        explicit TextureManager(Core::Renderer::IRenderer& renderer);

        Core::Renderer::TextureID GetTexture(Core::Renderer::TextureID id) const;
        Core::Renderer::TextureID CreateTexture(TextureLoader::TextureHandle texture);

    private:
        std::vector<Core::Renderer::TextureID> m_textures;

    private:
        Core::Renderer::IRenderer& m_renderer;
    };
}