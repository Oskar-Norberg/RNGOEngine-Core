//
// Created by Oskar.Norberg on 2025-09-03.
//

#pragma once

#include <expected>
#include <filesystem>

namespace RNGOEngine::AssetHandling::TextureLoader
{
    struct TextureData
    {
        unsigned int width;
        unsigned int height;
        unsigned int nrChannels;

        unsigned char* data;
    };
    
    struct TextureHandle
    {
        // TODO: This should probably be wrapped in a smart pointer with a custom deletor.
        TextureData* data;
    };

    enum class TextureLoadingError
    {
        FileNotFound,
        FailedToLoad,
    };

    std::expected<TextureHandle, TextureLoadingError> LoadTexture(const std::filesystem::path& path);
    void FreeTexture(TextureHandle texture);
}