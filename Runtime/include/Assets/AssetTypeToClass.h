//
// Created by Oskar.Norberg on 2025-11-24.
//

#pragma once

#include "Asset.h"
#include "AssetTypes/ModelAsset.h"
#include "AssetTypes/ShaderAsset.h"
#include "AssetTypes/TextureAsset.h"
#include "Utilities/Concepts/Concepts.h"

namespace RNGOEngine::AssetHandling
{
    template<AssetType>
    struct ClassForAssetType;

    // TODO: It's ugly, but whenever we need to make a new AssetType.
    // Just add a mapping of its class to Type here.
    template<>
    struct ClassForAssetType<AssetType::Model>
    {
        using Type = ModelAsset;
    };

    template<>
    struct ClassForAssetType<AssetType::Texture>
    {
        using Type = TextureAsset;
    };

    template<>
    struct ClassForAssetType<AssetType::Shader>
    {
        using Type = ShaderAsset;
    };

    template<>
    struct ClassForAssetType<AssetType::Material>
    {
        using Type = MaterialAsset;
    };
}
