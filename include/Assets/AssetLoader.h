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
        explicit AssetLoader(AssetDatabase& assetDatabase, AssetFetcher& assetFetcher);

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

        // TODO: TEMPORARY
    public:
        template<Concepts::DerivedFrom<AssetImporter> TAssetImporter>
        TAssetImporter* GetImporter(const AssetType type) const
        {
            const auto it = m_loaders.find(type);
            if (it != m_loaders.end())
            {
                return dynamic_cast<TAssetImporter*>(it->second.get());
            }

            return nullptr;
        }

    private:
        AssetDatabase& m_assetDatabase;
        AssetFetcher& m_assetFetcher;
        std::unordered_map<AssetType, std::unique_ptr<AssetImporter>> m_loaders;
    };
}