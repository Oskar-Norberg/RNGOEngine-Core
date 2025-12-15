//
// Created by Oskar.Norberg on 2025-11-03.
//

#pragma once

#include "AssetSerializer.h"
#include "Assets/AssetTypes/TextureAsset.h"
#include "magic_enum/magic_enum.hpp"

namespace RNGOEngine::AssetHandling
{
    constexpr int TEXTURE_METADATA_VERSION = 1;

    class TextureAssetSerializer : public AssetSerializer
    {
    public:
        std::unique_ptr<AssetMetadata> Deserialize(
            YAML::Node& node, const std::filesystem::path& assetPath
        ) override
        {
            auto metadata = std::make_unique<TextureMetadata>();
            DeserializeCommon(node, *metadata, assetPath);
            DeserializeImpl(node, *metadata);

            return std::move(metadata);
        }

    protected:
        void SerializeImpl(const AssetMetadata& metadata, YAML::Emitter& emitter) override
        {
            const auto* textureMetadataPtr = dynamic_cast<const TextureMetadata*>(&metadata);
            RNGO_ASSERT(textureMetadataPtr && "Failed to cast AssetMetadata to TextureMetadata.");
            const auto& textureMetadata = *textureMetadataPtr;

            emitter << YAML::Key << "Texture Metadata Version" << YAML::Value << TEXTURE_METADATA_VERSION;

            emitter << YAML::Key << "Format" << YAML::Value
                    << magic_enum::enum_name(textureMetadata.Format).data();

            emitter << YAML::Key << "MinifyingFilter" << YAML::Value
                    << magic_enum::enum_name(textureMetadata.MinifyingFilter).data();

            emitter << YAML::Key << "MagnifyingFilter" << YAML::Value
                    << magic_enum::enum_name(textureMetadata.MagnifyingFilter).data();

            emitter << YAML::Key << "WrappingMode" << YAML::Value
                    << magic_enum::enum_name(textureMetadata.WrappingMode).data();
        }

        void DeserializeImpl(YAML::Node& node, AssetMetadata& outMetadata) override
        {
            auto* textureMetadataPtr = dynamic_cast<TextureMetadata*>(&outMetadata);
            RNGO_ASSERT(textureMetadataPtr && "Failed to cast AssetMetadata to TextureMetadata.");
            auto& textureMetadata = *textureMetadataPtr;

            const auto textureMetadataVersion = node["Texture Metadata Version"].as<int>();
            RNGO_ASSERT(
                textureMetadataVersion == TEXTURE_METADATA_VERSION &&
                "Texture metadata version mismatch during deserialization."
            );

            const auto format =
                magic_enum::enum_cast<Core::Renderer::TextureFormat>(node["Format"].as<std::string>())
                    .value_or(Core::Renderer::TextureFormat::RGB);
            textureMetadata.Format = format;

            const auto minifyingFilter = magic_enum::enum_cast<Core::Renderer::TextureFiltering>(
                                             node["MinifyingFilter"].as<std::string>()
            )
                                             .value_or(Core::Renderer::TextureFiltering::LINEAR);
            textureMetadata.MinifyingFilter = minifyingFilter;

            const auto magnifyingFilter = magic_enum::enum_cast<Core::Renderer::TextureFiltering>(
                                              node["MagnifyingFilter"].as<std::string>()
            )
                                              .value_or(Core::Renderer::TextureFiltering::LINEAR);
            textureMetadata.MagnifyingFilter = magnifyingFilter;

            const auto wrappingMode =
                magic_enum::enum_cast<Core::Renderer::TextureWrapping>(node["WrappingMode"].as<std::string>())
                    .value_or(Core::Renderer::TextureWrapping::REPEAT);
            textureMetadata.WrappingMode = wrappingMode;
        }
    };
}