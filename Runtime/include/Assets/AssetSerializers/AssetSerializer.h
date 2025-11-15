//
// Created by Oskar.Norberg on 2025-11-03.
//

#pragma once

#include <yaml-cpp/yaml.h>

#include "Assets/Asset.h"

namespace RNGOEngine::AssetHandling
{
    // TODO: NEED VERSIONING SYSTEM. METADATA SHITS ITSELF WHEN I ADD/REMOVE FIELDS.
    class AssetSerializer
    {
    public:
        virtual ~AssetSerializer() = default;

        virtual void Serialize(const AssetMetadata& metadata, YAML::Emitter& emitter);
        virtual std::unique_ptr<AssetMetadata> Deserialize(YAML::Node& node, const std::filesystem::path& assetPath) = 0;
    };
}