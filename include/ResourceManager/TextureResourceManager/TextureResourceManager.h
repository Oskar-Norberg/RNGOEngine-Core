//
// Created by Oskar.Norberg on 2025-10-14.
//

#pragma once
#include "Renderer/RenderID.h"
#include "Renderer/Handles/TextureHandle.h"

namespace RNGOEngine::Core::Renderer
{
    class IRenderer;
}

namespace RNGOEngine::Resources
{
    // TODO: Wrap everything in generational handles.
    class TextureResourceManager
    {
    public:
        explicit TextureResourceManager(RNGOEngine::Core::Renderer::IRenderer& renderer);

    public:
        Core::Renderer::TextureID CreateTexture(AssetHandling::Textures::TextureHandle textureHandle);
        void DestroyTexture(Core::Renderer::TextureID texture);

    private:
        RNGOEngine::Core::Renderer::IRenderer& m_renderer;
    };
}
