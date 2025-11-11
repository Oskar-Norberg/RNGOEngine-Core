//
// Created by Oskar.Norberg on 2025-09-03.
//

#pragma once

#include <expected>
#include <filesystem>

namespace RNGOEngine::AssetHandling::TextureLoader
{
    enum class TextureLoadingError
    {
        None,
        FileNotFound,
        FailedToLoad,
    };

    struct TextureData
    {
        unsigned int width;
        unsigned int height;
        unsigned int nrChannels;

        // TODO: Wrap in unique ptr.
        unsigned char* data;
    };

    std::expected<TextureData, TextureLoadingError> LoadTexture(const std::filesystem::path& path);
    void FreeTexture(const TextureData& texture);
}