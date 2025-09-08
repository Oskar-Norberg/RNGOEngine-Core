//
// Created by Oskar.Norberg on 2025-09-03.
//

#include "AssetManager/AssetLoaders/TextureLoader.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

namespace RNGOEngine::AssetHandling
{
    TextureLoader::TextureLoader(Core::Renderer::IRenderer& renderer, const AssetFileFetcher& fileFetcher,
                                 bool doFlipVertically)
        : m_renderer(renderer), m_fileFetcher(fileFetcher)
    {
        stbi_set_flip_vertically_on_load(doFlipVertically);
    }

    Core::Renderer::TextureID TextureLoader::LoadTexture(const std::filesystem::path& path) const
    {
        // TODO: Move this to the AssetManager. The TextureLoader should not fetch the path.
        const auto foundPath = m_fileFetcher.GetTexturePath(path);

        if (!foundPath.has_value())
        {
            assert(false && "Texture not found.");
            return Core::Renderer::INVALID_TEXTURE_ID;
        }

        int width, height, nrChannels;
        unsigned char* data = stbi_load(foundPath.value().string().data(), &width, &height, &nrChannels, 0);

        if (!data || width <= 0 || height <= 0 || nrChannels <= 0)
        {
            assert(false && "Failed to load texture");
            return Core::Renderer::INVALID_TEXTURE_ID;
        }

        const auto textureHandle = m_renderer.CreateTexture(data, width, height, nrChannels);
        stbi_image_free(data);

        return textureHandle;
    }
}