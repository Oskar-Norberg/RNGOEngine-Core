//
// Created by Oskar.Norberg on 2025-10-14.
//

#pragma once

#include "Renderer/RenderID.h"
#include "Renderer/Handles/TextureHandle.h"
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
        Containers::Vectors::GenerationalKey<Core::Renderer::TextureID> CreateTexture(AssetHandling::Textures::TextureHandle textureHandle);
        void MarkTextureForDeletion(Containers::Vectors::GenerationalKey<Core::Renderer::TextureID> key);

        void DeleteMarkedTextures();

        // Accessors
    public:
        std::optional<Core::Renderer::TextureID> GetTexture(const Containers::Vectors::GenerationalKey<Core::Renderer::TextureID>& key) const;

    private:
        Containers::Vectors::GenerationalVector<Core::Renderer::TextureID> m_textures;

    private:
        RNGOEngine::Core::Renderer::IRenderer& m_renderer;
    };
}