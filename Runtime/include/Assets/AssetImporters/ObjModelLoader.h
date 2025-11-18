//
// Created by Oskar.Norberg on 2025-11-18.
//

#pragma once

#include "AssetImporter.h"

namespace RNGOEngine::AssetHandling
{
    class ObjModelLoader : public AssetImporter
    {
    public:
        void Load(const AssetMetadata& metadata) override;
        
        void Unload(const AssetHandle& handle) override;
        
        std::unique_ptr<AssetMetadata> CreateDefaultMetadata(
            const std::filesystem::path& path
        ) const override;
        
        std::span<const std::string_view> GetSupportedExtensions() const override;
    };
}