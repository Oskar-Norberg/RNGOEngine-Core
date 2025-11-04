//
// Created by ringo on 2025-11-01.
//

#pragma once

#include "AssetTypes/MaterialAsset.h"
#include "AssetTypes/ModelAsset.h"
#include "AssetTypes/ShaderAsset.h"
#include "AssetTypes/TextureAsset.h"

namespace RNGOEngine::AssetHandling
{
    template<typename Metadata>
    struct AssetTypeForMetadata;

    template<>
    struct AssetTypeForMetadata<ModelMetadata>
    {
        static constexpr auto value = AssetType::Model;
    };

    template<>
    struct AssetTypeForMetadata<TextureMetadata>
    {
        static constexpr auto value = AssetType::Texture;
    };

    template<>
    struct AssetTypeForMetadata<ShaderMetadata>
    {
        static constexpr auto value = AssetType::Shader;
    };

    template<>
    struct AssetTypeForMetadata<MaterialMetadata>
    {
        static constexpr auto value = AssetType::Material;
    };
}