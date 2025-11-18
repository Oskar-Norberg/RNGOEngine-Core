//
// Created by ringo on 2025-11-02.
//

#pragma once

#include "AssetImporter.h"

namespace RNGOEngine::AssetHandling
{
    class AssimpModelImporter : public AssetImporter
    {
    public:
        // TODO: This ugly construct wouldn't have to be here if they were just singletons...
        explicit AssimpModelImporter(bool doFlipUVs);

        void Load(const AssetMetadata& metadata) override;
        void Unload(const AssetHandle& handle) override;

        std::unique_ptr<AssetMetadata> CreateDefaultMetadata(const std::filesystem::path& path) const override;

        std::span<const std::string_view> GetSupportedExtensions() const override;

    private:
        bool m_doFlipUVs;
    };
}