//
// Created by Oskar.Norberg on 2025-10-14.
//

#pragma once

#include <span>

#include "Renderer/RenderID.h"
#include "Utilities/Containers/GenerationalVector/GenerationalVector.h"

namespace RNGOEngine::Core::Renderer
{
    class IRenderer;
}

namespace RNGOEngine::Resources
{
    class TextureResourceManager
    {
    public:
        explicit TextureResourceManager(RNGOEngine::Core::Renderer::IRenderer& renderer);

        // Creation / Destruction
    public:
        Containers::GenerationalKey<Core::Renderer::TextureID> CreateTexture(const Core::Renderer::Texture2DProperties& properties, std::span<const std::byte> textureData);
        void MarkTextureForDeletion(Containers::GenerationalKey<Core::Renderer::TextureID> key);

        // Accessors
    public:
        std::optional<Core::Renderer::TextureID> GetTexture(const Containers::GenerationalKey<Core::Renderer::TextureID>& key) const;

        // Engine Internals
    public:
        void MarkAllTextures();
        void DestroyMarkedTextures();

    private:
        Containers::GenerationalVector<Core::Renderer::TextureID> m_textures;

    private:
        RNGOEngine::Core::Renderer::IRenderer& m_renderer;
    };
}