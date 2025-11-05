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
        void Serialize(const AssetMetadata& metadata, YAML::Emitter& emitter) override
        {
            AssetSerializer::Serialize(metadata, emitter);

            // TODO: Potentially questionable downcast.
            // const auto& textureMetadata = static_cast<const TextureMetadata&>(metadata);

            // Texture has no specific fields yet.
        }

        std::unique_ptr<AssetMetadata>
        Deserialize(YAML::Node& node, const std::filesystem::path& assetPath) override
        {
            const auto uuidVal = node["UUID"].as<uint64_t>();
            const auto type = static_cast<AssetType>(node["Type"].as<int>());

            Utilities::UUID uuid = Utilities::UUID(uuidVal);
            auto metadata = std::make_unique<TextureMetadata>();
            metadata->UUID = uuid;
            metadata->Type = type;
            metadata->Path = assetPath;

            return std::move(metadata);
        }
    };
}