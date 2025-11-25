//
// Created by ringo on 2025-11-02.
//

#pragma once

#include "AssetImporter.h"

namespace RNGOEngine::AssetHandling
{
    class ModelImporter : public AssetImporter
    {
    public:
        explicit ModelImporter(bool doFlipUVs);

        std::expected<std::unique_ptr<Asset>, ImportingError> Load(const AssetMetadata& metadata) override;
        void Unload(const AssetHandle& handle) override;

        std::unique_ptr<AssetMetadata> CreateDefaultMetadata(
            const std::filesystem::path& path
        ) const override;

        std::span<const std::string_view> GetSupportedExtensions() const override;

    private:
        bool m_doFlipUVs;
    };
}