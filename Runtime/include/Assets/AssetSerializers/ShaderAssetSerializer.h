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
        }

        std::unique_ptr<AssetMetadata> Deserialize(YAML::Node& node, const std::filesystem::path& assetPath) override
        {
            const auto uuidVal = node["UUID"].as<uint64_t>();
            const auto type = static_cast<AssetType>(node["Type"].as<int>());

            auto metadata = std::make_unique<ShaderMetadata>();
            metadata->UUID = Utilities::UUID(uuidVal);
            metadata->Type = type;
            metadata->Path = assetPath;


            return std::move(metadata);
        }
    };
}