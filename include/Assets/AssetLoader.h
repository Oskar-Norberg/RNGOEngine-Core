//
// Created by ringo on 2025-11-02.
//

#pragma once

#include <filesystem>

#include "Asset.h"
#include "AssetFetcher/AssetFetcher.h"
#include "AssetImporters/AssetImporter.h"
#include "Concepts/Concepts.h"

namespace RNGOEngine::AssetHandling
{
    class AssetLoader
    {
    public:
        explicit AssetLoader(AssetFetcher& m_asset_fetcher);

        AssetHandle Load(AssetType type, const std::filesystem::path& path) const;

        // Loader Registration
    public:
        template<Concepts::DerivedFrom<AssetImporter> TAssetImporter, typename... Args>
        void RegisterImporter(const AssetType type, Args&&... args)
        {
            m_loaders[type] = std::make_unique<TAssetImporter>(std::forward<Args>(args)...);
        }

        template<typename T>
        void UnregisterImporter(const AssetType type)
        {
            m_loaders.erase(type);
        }

    private:
        AssetFetcher& m_assetFetcher;
        std::unordered_map<AssetType, std::unique_ptr<AssetImporter>> m_loaders;
    };
}