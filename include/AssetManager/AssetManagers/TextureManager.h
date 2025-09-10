//
// Created by Oskar.Norberg on 2025-09-10.
//

#pragma once

#include <unordered_map>
#include <vector>

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

        Core::Renderer::TextureID GetTexture(Core::Renderer::TextureID id) const;
        std::optional<Core::Renderer::TextureID> GetTextureIfLoaded(const std::filesystem::path& path) const;
        Core::Renderer::TextureID CreateTexture(TextureLoader::TextureHandle texture);

    private:
        std::unordered_map<std::filesystem::path, Core::Renderer::TextureID> m_loadedTexturePaths;
        std::vector<Core::Renderer::TextureID> m_textures;

    private:
        Core::Renderer::IRenderer& m_renderer;
    };
}