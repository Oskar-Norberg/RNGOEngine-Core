//
// Created by ringo on 2025-11-02.
//

#include "Assets/AssetLoader.h"

#include "Utilities/RNGOAsserts.h"

namespace RNGOEngine::AssetHandling
{
    AssetLoader::AssetLoader(AssetFetcher& m_asset_fetcher)
        : m_assetFetcher(m_asset_fetcher)
    {
    }

    AssetHandle AssetLoader::Load(const AssetType type, const std::filesystem::path& path) const
    {
        RNGO_ASSERT(m_loaders.contains(type) && "No loader registered for the specified asset type.");

        const auto fullPath = m_assetFetcher.GetPath(type, path);
        if (!fullPath)
        {
            RNGO_ASSERT(false && "AssetLoader::Load - Asset not found!");
            // TODO: What to return here?
            // TODO: UB
        }

        return m_loaders.at(type)->Load(fullPath.value());
    }
}