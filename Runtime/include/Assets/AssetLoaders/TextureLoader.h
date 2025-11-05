//
// Created by Oskar.Norberg on 2025-09-03.
//

#pragma once

#include <expected>
#include <filesystem>

#include "Renderer/Handles/TextureHandle.h"

namespace RNGOEngine::AssetHandling::TextureLoader
{
    enum class TextureLoadingError
    {
        None,
        FileNotFound,
        FailedToLoad,
    };

    std::expected<Textures::TextureHandle, TextureLoadingError> LoadTexture(const std::filesystem::path& path);
    void FreeTexture(Textures::TextureHandle texture);
}