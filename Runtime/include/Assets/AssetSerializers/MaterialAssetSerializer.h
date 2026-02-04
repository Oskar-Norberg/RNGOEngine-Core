//
// Created by Oskar.Norberg on 2026-02-04.
//

#pragma once

#include "AssetSerializer.h"
#include "Assets/AssetDatabase/AssetDatabase.h"
#include "Assets/AssetTypes/ModelAsset.h"

namespace RNGOEngine::AssetHandling
{
    class MaterialAssetSerializer : public AssetSerializer
    {
    public:
        std::unique_ptr<AssetMetadata> Deserialize(
            YAML::Node& node, const std::filesystem::path& assetPath
        ) override
        {
            auto metadata = std::make_unique<MaterialMetadata>();
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