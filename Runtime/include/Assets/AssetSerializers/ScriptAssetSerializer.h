//
// Created by Oskar.Norberg on 2026-01-22.
//

#pragma once

#include "AssetSerializer.h"
#include "Assets/AssetTypes/ScriptAsset.h"

namespace RNGOEngine::AssetHandling
{
    class ScriptAssetSerializer : public AssetSerializer
    {
    public:
        std::unique_ptr<AssetMetadata> Deserialize(
            YAML::Node& node, const std::filesystem::path& assetPath
        ) override
        {
            auto metadata = std::make_unique<ScriptMetadata>();
            DeserializeCommon(node, *metadata, assetPath);
            DeserializeImpl(node, *metadata);
            return std::move(metadata);
        }

    protected:
        void SerializeImpl(const AssetMetadata& metadata, YAML::Emitter& emitter) override
        {
        }
        void DeserializeImpl(YAML::Node& node, AssetMetadata& outMetadata) override
        {
        }
    };
}