//
// Created by Oskar.Norberg on 2025-09-10.
//

#pragma once

#include "AssetManager/AssetLoaders/TextureLoader.h"
#include "Renderer/RenderID.h"

namespace RNGOEngine::Core::Renderer
{
    class IRenderer;
}

namespace Core
{
    namespace Renderer
    {
        class IRenderer;
    }
}

namespace RNGOEngine::AssetHandling
{
    class TextureManager
    {
    public:
        explicit TextureManager(Core::Renderer::IRenderer& renderer);
        
        Core::Renderer::TextureID CreateTexture(TextureLoader::TextureHandle texture);

    private:
        Core::Renderer::IRenderer& m_renderer;
    };
}