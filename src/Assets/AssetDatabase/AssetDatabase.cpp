//
// Created by Oskar.Norberg on 2025-10-24.
//

#include "Assets/AssetDatabase/AssetDatabase.h"

namespace RNGOEngine::AssetHandling
{
    AssetDatabase::AssetDatabase()
        : Singleton(this)
    {
    }

    void AssetDatabase::RegisterAsset(AssetType type, std::unique_ptr<AssetMetadata> metadata)
    {
        if (!m_metadataStorages.contains(type))
        {
            m_metadataStorages[type] = CreateTyped(type);
        }

        const auto uuid = metadata->UUID;
        const auto path = metadata->Path;
        
        const auto& storage = m_metadataStorages.at(type);
        const auto index = storage->Insert(std::move(metadata));

        m_handleToStorageIndex.insert({uuid, std::make_pair(type, index)});
        m_pathToHandle.insert({path, uuid});
    }

    void AssetDatabase::UnregisterAsset(const AssetHandle& uuid)
    {
        if (!m_handleToStorageIndex.contains(uuid))
        {
            return;
        }

        const auto& [type, index] = m_handleToStorageIndex.at(uuid);
        const auto& storage = m_metadataStorages.at(type);
        const auto path = storage->GetAssetMetadata(index)->Path;
        storage->Remove(index);
        m_handleToStorageIndex.erase(uuid);
        m_pathToHandle.erase(path);
    }

    bool AssetDatabase::IsRegistered(const AssetHandle& handle) const
    {
        const auto* metadata = GetMetadataOrNullptr(handle);

        if (metadata)
        {
            return metadata->State != AssetState::None;
        }

        return false;
    }

    AssetState AssetDatabase::GetAssetState(const AssetHandle& handle) const
    {
        const auto* metadata = GetMetadataOrNullptr(handle);
        if (metadata)
        {
            return metadata->State;
        }

        return AssetState::None;
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
        if (m_handleToStorageIndex.contains(handle))
        {
            const auto& [type, index] = m_handleToStorageIndex.at(handle);
            const auto& storage = m_metadataStorages.at(type);
            return storage->GetAssetMetadata(index);
        }

        return nullptr;
    }
}