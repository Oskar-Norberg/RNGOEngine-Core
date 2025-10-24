//
// Created by Oskar.Norberg on 2025-09-03.
//

#include "AssetHandling/AssetLoaders/TextureLoader.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "Utilities/RNGOAsserts.h"

namespace RNGOEngine::AssetHandling::TextureLoader
{
    std::expected<Textures::TextureHandle, TextureLoadingError> LoadTexture(
        const std::filesystem::path& path)
    {
        int width, height, nrChannels;
        unsigned char* data = stbi_load(path.string().data(), &width, &height, &nrChannels, 0);
        
        if (!data || width <= 0 || height <= 0 || nrChannels <= 0)
        {
            RNGO_ASSERT(false && "Failed to load texture");
            return std::unexpected(TextureLoadingError::FailedToLoad);
        }

        return Textures::TextureHandle{new Textures::TextureData{
                static_cast<unsigned int>(width),
                static_cast<unsigned int>(height),
                static_cast<unsigned int>(nrChannels),
                std::move(path).string(),
                data
            }
        };
    }

    void FreeTexture(const Textures::TextureHandle texture)
    {
        stbi_image_free(texture.data->data);
        delete texture.data;
    }
}