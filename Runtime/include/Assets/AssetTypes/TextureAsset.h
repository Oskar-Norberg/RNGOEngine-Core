//
// Created by ringo on 2025-10-31.
//

#pragma once
#include "Assets/Asset.h"

namespace RNGOEngine::AssetHandling
{
    // TODO: This is strictly a 2D texture for now.
    struct TextureMetadata : AssetMetadata
    {
        Core::Renderer::TextureFormat Format;
        Core::Renderer::TextureFiltering MinifyingFilter;
        Core::Renderer::TextureFiltering MagnifyingFilter;
        Core::Renderer::TextureWrapping WrappingMode;
    };
}