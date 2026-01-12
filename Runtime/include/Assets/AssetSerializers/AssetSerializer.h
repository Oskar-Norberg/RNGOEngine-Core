//
// Created by Oskar.Norberg on 2025-11-03.
//

#pragma once

#include <yaml-cpp/yaml.h>

#include "Assets/Asset.h"

namespace RNGOEngine::AssetHandling
{
    constexpr int METADATA_VERSION = 1;

    class AssetSerializer
    {
    public:
        virtual ~AssetSerializer() = default;

        // API
    public:
        void Serialize(const AssetMetadata& metadata, YAML::Emitter& emitter);

        virtual std::unique_ptr<AssetMetadata> Deserialize(YAML::Node& node, const std::filesystem::path& assetPath) = 0;

        // Common
    protected:
        void SerializeCommon(const AssetMetadata& metadata, YAML::Emitter& emitter);
        void DeserializeCommon(YAML::Node& node, AssetMetadata& outMetadata, const std::filesystem::path& assetPath);

        // Implementation
    protected:
        virtual void SerializeImpl(const AssetMetadata& metadata, YAML::Emitter& emitter) = 0;
        virtual void DeserializeImpl(YAML::Node& node, AssetMetadata& outMetadata) = 0;
    };
}