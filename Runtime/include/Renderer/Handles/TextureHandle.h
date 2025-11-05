//
// Created by ringo on 2025-10-04.
//

#pragma once

#include <filesystem>

namespace RNGOEngine::AssetHandling::Textures
{
    struct TextureData
    {
        unsigned int width;
        unsigned int height;
        unsigned int nrChannels;

        std::filesystem::path path;
        unsigned char* data;
    };

    struct TextureHandle
    {
        // TODO: This should probably be wrapped in a smart pointer with a custom deletor.
        TextureData* data;
    };
}