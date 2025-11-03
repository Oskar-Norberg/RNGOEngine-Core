//
// Created by Oskar.Norberg on 2025-11-03.
//

#pragma once

#include "AssetSerializer.h"
#include "Assets/AssetTypes/TextureAsset.h"

namespace RNGOEngine::AssetHandling
{
    class TextureAssetSerializer : public AssetSerializer
    {
    public:
        void Serialize(AssetMetadata& metadata, YAML::Emitter& emitter) override
        {
            AssetSerializer::Serialize(metadata, emitter);

            // TODO: Potentially questionable downcast.
            // const auto& textureMetadata = static_cast<const TextureMetadata&>(metadata);

            // Texture has no specific fields yet.
        }
    };
}