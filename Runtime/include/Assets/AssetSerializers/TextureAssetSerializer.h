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
            const auto& textureMetadata = static_cast<const TextureMetadata&>(metadata);

            emitter << YAML::BeginMap;
            emitter << YAML::Key << "Format" << YAML::Value << static_cast<int>(textureMetadata.Format);
            emitter << YAML::Key << "Minifying Filter" << YAML::Value << static_cast<int>(textureMetadata.MinifyingFilter);
            emitter << YAML::Key << "Magnifying Filter" << YAML::Value << static_cast<int>(textureMetadata.MagnifyingFilter);
            emitter << YAML::Key << "Wrapping Mode" << YAML::Value << static_cast<int>(textureMetadata.WrappingMode);
            emitter << YAML::EndMap;
        }

        std::unique_ptr<AssetMetadata>
        Deserialize(YAML::Node& node, const std::filesystem::path& assetPath) override
        {
            const auto uuidVal = node["UUID"].as<uint64_t>();
            const auto type = static_cast<AssetType>(node["Type"].as<int>());

            auto uuid = Utilities::UUID(uuidVal);
            auto metadata = std::make_unique<TextureMetadata>();
            metadata->UUID = uuid;
            metadata->Type = type;
            metadata->Path = assetPath;
            metadata->Format = static_cast<Core::Renderer::TextureFormat>(node["Format"].as<int>());
            metadata->MinifyingFilter = static_cast<Core::Renderer::TextureFiltering>(node["Minifying Filter"].as<int>());
            metadata->MagnifyingFilter = static_cast<Core::Renderer::TextureFiltering>(node["Magnifying Filter"].as<int>());
            metadata->WrappingMode = static_cast<Core::Renderer::TextureWrapping>(node["Wrapping Mode"].as<int>());

            return std::move(metadata);
        }
    };
}