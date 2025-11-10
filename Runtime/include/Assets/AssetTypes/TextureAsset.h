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
        Core::Renderer::TextureFormat format;
        Core::Renderer::TextureFiltering minifyingFilter;
        Core::Renderer::TextureFiltering magnifyingFilter;
        Core::Renderer::TextureWrapping wrappingMode;
    };
}