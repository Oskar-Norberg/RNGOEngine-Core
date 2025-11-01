//
// Created by Oskar.Norberg on 2025-10-24.
//

#include "Assets/AssetDatabase/AssetDatabase.h"

namespace RNGOEngine::AssetHandling
{
    bool AssetDatabase::IsRegistered(const AssetHandle& handle) const
    {
        return m_assetMetadataMap.contains(handle);
    }

    bool AssetDatabase::IsRegistered(const std::filesystem::path& path) const
    {
        return m_pathToHandle.contains(path);
    }

    AssetHandle AssetDatabase::GetAssetHandle(const std::filesystem::path& path) const
    {
        // Unsafe method! Assumes path is registered.
        return m_pathToHandle.at(path);
    }

    AssetState AssetDatabase::GetAssetState(const AssetHandle& handle) const
    {
        if (m_assetMetadataMap.contains(handle))
        {
            return m_assetMetadataMap.at(handle)->State;
        }

        return AssetState::None;
    }

    AssetMetadata& AssetDatabase::GetAssetMetadata(const AssetHandle& handle)
    {
        // Unsafe method!
        return *m_assetMetadataMap.at(handle).get();
    }
}