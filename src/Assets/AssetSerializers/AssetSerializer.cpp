//
// Created by Oskar.Norberg on 2025-11-03.
//

#include "Assets/AssetSerializers/AssetSerializer.h"

namespace RNGOEngine::AssetHandling
{
    void AssetSerializer::Serialize(const AssetMetadata& metadata, YAML::Emitter& emitter)
    {
        emitter << YAML::BeginMap;
        emitter << YAML::Key << "UUID" << YAML::Value << metadata.UUID.GetValue();
        emitter << YAML::Key << "Type" << YAML::Value << static_cast<int>(metadata.Type);
        emitter << YAML::EndMap;
    }
}