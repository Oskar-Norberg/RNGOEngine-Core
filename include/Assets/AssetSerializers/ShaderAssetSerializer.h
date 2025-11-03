//
// Created by Oskar.Norberg on 2025-11-03.
//

#pragma once

#include "AssetSerializer.h"
#include "Assets/AssetTypes/ShaderAsset.h"

namespace RNGOEngine::AssetHandling
{
    class ShaderAssetSerializer : public AssetSerializer
    {
    public:
        void Serialize(const AssetMetadata& metadata, YAML::Emitter& emitter) override
        {
            AssetSerializer::Serialize(metadata, emitter);

            const auto& shaderMetadata = static_cast<const ShaderMetadata&>(metadata);
            emitter << YAML::Key << "ShaderType" << YAML::Value
                    << static_cast<int>(shaderMetadata.ShaderType);
        }
    };
}