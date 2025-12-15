//
// Created by Oskar.Norberg on 2025-11-03.
//

#include "Assets/AssetSerializers/AssetSerializer.h"

#include "magic_enum/magic_enum.hpp"

namespace RNGOEngine::AssetHandling
{
    void AssetSerializer::Serialize(const AssetMetadata& metadata, YAML::Emitter& emitter)
    {
        emitter << YAML::BeginMap;
        SerializeCommon(metadata, emitter);
        SerializeImpl(metadata, emitter);
        emitter << YAML::EndMap;
    }

    void AssetSerializer::SerializeCommon(const AssetMetadata& metadata, YAML::Emitter& emitter)
    {
        emitter << YAML::Key << "Version" << YAML::Value << METADATA_VERSION;
        emitter << YAML::Key << "UUID" << YAML::Value << metadata.UUID.GetValue();
        emitter << YAML::Key << "Type" << YAML::Value << magic_enum::enum_name(metadata.Type).data();
    }

    void AssetSerializer::DeserializeCommon(
        YAML::Node& node, AssetMetadata& outMetadata, const std::filesystem::path& assetPath
    )
    {
        outMetadata.UUID = Utilities::UUID(node["UUID"].as<uint64_t>());

        const AssetType type =
            magic_enum::enum_cast<AssetType>(node["Type"].as<std::string>()).value_or(AssetType::None);
        outMetadata.Type = type;

        outMetadata.Path = assetPath;
    }
}