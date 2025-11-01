//
// Created by Oskar.Norberg on 2025-10-24.
//

#include "Assets/AssetDatabase/AssetDatabase.h"

namespace RNGOEngine::AssetHandling
{
    bool AssetDatabase::IsRegistered(const AssetHandle& handle) const
    {
        return m_assetMetadataMap.contains(handle)
               && m_assetMetadataMap.at(handle).get()->State != AssetState::None;
    }

    AssetState AssetDatabase::GetAssetState(const AssetHandle& handle) const
    {
        return m_assetMetadataMap.contains(handle)
                   ? m_assetMetadataMap.at(handle).get()->State
                   : AssetState::None;
    }

    bool AssetDatabase::IsRegistered(const std::filesystem::path& path) const
    {
        return m_pathToHandle.contains(path);
    }

    AssetHandle AssetDatabase::GetAssetHandle(const std::filesystem::path& path) const
    {
        // Unsafe.
        return m_pathToHandle.at(path);
    }

    AssetMetadata& AssetDatabase::GetAssetMetadata(const AssetHandle& handle)
    {
        // Unsafe.
        return const_cast<AssetMetadata&>(static_cast<const AssetDatabase*>(this)->GetAssetMetadata(handle));
    }

    const AssetMetadata& AssetDatabase::GetAssetMetadata(const AssetHandle& handle) const
    {
        // Unsafe.
        return *GetMetadataOrNullptr(handle);
    }

    std::optional<AssetHandle> AssetDatabase::TryGetAssetHandle(const std::filesystem::path& path) const
    {
        if (m_pathToHandle.contains(path))
        {
            if (const auto* metadata = GetMetadataOrNullptr(m_pathToHandle.at(path)); metadata != nullptr)
            {
                return metadata->UUID;
            }
        }

        return std::nullopt;
    }

    std::optional<std::reference_wrapper<AssetMetadata>> AssetDatabase::TryGetAssetMetadata(
        const AssetHandle& handle)
    {
        const auto constThis = static_cast<const AssetDatabase*>(this);
        const auto assetMetadataOpt = constThis->TryGetAssetMetadata(handle);
        if (assetMetadataOpt.has_value())
        {
            return std::ref(const_cast<AssetMetadata&>(assetMetadataOpt->get()));
        }

        return std::nullopt;
    }

    std::optional<std::reference_wrapper<const AssetMetadata>> AssetDatabase::TryGetAssetMetadata(
        const AssetHandle& handle) const
    {
        if (const auto* metadata = GetMetadataOrNullptr(handle); metadata != nullptr)
        {
            return std::cref(*metadata);
        }

        return std::nullopt;
    }

    const AssetMetadata* AssetDatabase::GetMetadataOrNullptr(const AssetHandle& handle) const
    {
        if (m_assetMetadataMap.contains(handle))
        {
            return m_assetMetadataMap.at(handle).get();
        }

        return nullptr;
    }
}