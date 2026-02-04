//
// Created by Oskar.Norberg on 2026-02-04.
//

#pragma once
#include "AssetImporter.h"

namespace RNGOEngine::AssetHandling
{
    class MaterialImporter : public AssetImporter
    {
    public:
        ImportingError LoadFromDisk(RuntimeAssetRegistry& registry, const AssetMetadata& metadata) override;
        ImportingError FinalizeLoad(Data::ThreadType threadType, RuntimeAssetRegistry& registry) override;

        void Unload(const AssetHandle& handle) override;

        std::unique_ptr<AssetMetadata> CreateDefaultMetadata(
            const std::filesystem::path& path
        ) const override;

        std::span<const std::string_view> GetSupportedExtensions() const override;

        Data::ThreadType GetFinalizationThreadTypes() const override
        {
            return Data::ThreadType::Render;
        }

        AssetType GetAssetType() const override
        {
            return AssetType::Material;
        }
    };
}