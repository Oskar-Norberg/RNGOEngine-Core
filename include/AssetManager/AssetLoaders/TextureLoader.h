//
// Created by Oskar.Norberg on 2025-09-03.
//

#pragma once

#include "AssetManager/AssetFileFetcher.h"
#include "Renderer/IRenderer.h"

namespace RNGOEngine::AssetHandling
{
    class TextureLoader
    {
    public:
        TextureLoader(Core::Renderer::IRenderer& renderer, const AssetFileFetcher& fileFetcher, bool doFlipVertically = true);

        Core::Renderer::TextureID LoadTexture(const std::filesystem::path& path) const;

    private:
        Core::Renderer::IRenderer& m_renderer;
        const AssetFileFetcher& m_fileFetcher;
    };
}