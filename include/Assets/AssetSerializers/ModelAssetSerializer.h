//
// Created by Oskar.Norberg on 2025-11-03.
//

#pragma once

#include "AssetSerializer.h"
#include "Assets/AssetTypes/ModelAsset.h"

namespace RNGOEngine::AssetHandling
{
    class ModelAssetSerializer : public AssetSerializer
    {
    public:
        void Serialize(const AssetMetadata& metadata, YAML::Emitter& emitter) override
        {
            AssetSerializer::Serialize(metadata, emitter);

            // TODO: Potentially questionable downcast.
            const auto& modelMetadata = static_cast<const ModelMetadata&>(metadata);
            // ModelMetadata has no specific fields yet.
        }
    };
}