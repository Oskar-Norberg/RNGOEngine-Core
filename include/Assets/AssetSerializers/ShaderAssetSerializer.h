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

        AssetHandle Deserialize(YAML::Node& node, const std::filesystem::path& assetPath) override
        {
            const auto uuidVal = node["UUID"].as<uint64_t>();
            const auto type = static_cast<AssetType>(node["Type"].as<int>());
            const auto shaderType = static_cast<Core::Renderer::ShaderType>(
                node["ShaderType"].as<int>());

            Utilities::UUID uuid = Utilities::UUID(uuidVal);
            ShaderMetadata metadata;
            metadata.UUID = uuid;
            metadata.Type = type;
            metadata.Path = assetPath;
            metadata.ShaderType = shaderType;

            AssetDatabase::GetInstance().RegisterAsset(metadata);

            return uuid;
        }
    };
}