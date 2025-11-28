//
// Created by ringo on 2025-11-02.
//

#pragma once

#include "AssetImporter.h"
#include "Assets/AssetTypes/TextureAsset.h"

namespace RNGOEngine::AssetHandling
{
    class TextureAssetImporter : public TAssetImporter<TextureAsset>
    {
    public:
        void Unload(const AssetHandle& handle) override;

        std::unique_ptr<AssetMetadata> CreateDefaultMetadata(
            const std::filesystem::path& path
        ) const override;

        std::span<const std::string_view> GetSupportedExtensions() const override;

    protected:
        std::expected<TextureAsset, ImportingError> ImportAsset(const AssetMetadata& metadata) override;
        AssetType GetAssetType() const override;
    };
}