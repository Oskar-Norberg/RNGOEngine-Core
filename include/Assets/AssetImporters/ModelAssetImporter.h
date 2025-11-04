//
// Created by ringo on 2025-11-02.
//

#pragma once

#include "AssetImporter.h"

namespace RNGOEngine::AssetHandling
{
    class ModelAssetImporter : public AssetImporter
    {
    public:
        // TODO: This ugly construct wouldn't have to be here if they were just singletons...
        explicit ModelAssetImporter(bool doFlipUVs);

        AssetHandle Register(const std::filesystem::path& path) override;
        void Unregister(const AssetHandle& handle) override;
        AssetHandle Load(const std::filesystem::path& path) override;
        void Unload(const AssetHandle& handle) override;

        std::span<const std::string_view> GetSupportedExtensions() const override;

    private:
        bool m_doFlipUVs;
    };
}