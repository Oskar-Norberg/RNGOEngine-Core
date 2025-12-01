//
// Created by ringo on 2025-11-02.
//

#pragma once

#include "AssetImporter.h"
#include "Assets/AssetTypes/ShaderAsset.h"
#include "Assets/AssetLoaders/ShaderLoader.h"

namespace RNGOEngine::AssetHandling
{
    class ShaderAssetImporter : public AssetImporter
    {
    public:
        ImportingError LoadFromDisk(RuntimeAssetRegistry& registry, const AssetMetadata& metadata) override;
        ImportingError FinalizeLoad(Data::ThreadType threadType, RuntimeAssetRegistry& registry) override;
        void Unload(const AssetHandle& handle) override;

        std::unique_ptr<AssetMetadata> CreateDefaultMetadata(
            const std::filesystem::path& path
        ) const override;

        std::span<const std::string_view> GetSupportedExtensions() const override;

    private:
        ShaderLoader m_shaderLoader;
    };
}