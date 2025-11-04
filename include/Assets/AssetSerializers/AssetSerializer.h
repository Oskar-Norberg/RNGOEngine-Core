//
// Created by Oskar.Norberg on 2025-11-03.
//

#pragma once

#include <yaml-cpp/yaml.h>

#include "Assets/Asset.h"

namespace RNGOEngine::AssetHandling
{
    class AssetSerializer
    {
    public:
        virtual ~AssetSerializer() = default;

        // Serialize into YAML::Emitter
        virtual void Serialize(const AssetMetadata& metadata, YAML::Emitter& emitter);
        // Deserialize from YAML::Node, register to DB and return AssetHandle
        virtual AssetHandle Deserialize(YAML::Node& node, const std::filesystem::path& assetPath) = 0;
    };
}