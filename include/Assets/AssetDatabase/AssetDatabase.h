//
// Created by Oskar.Norberg on 2025-10-24.
//

#pragma once

#include <filesystem>

#include "Assets/Asset.h"
#include "Assets/AssetMetadataStorage.h"
#include  "Assets/AssetMetadataTypes.h"
#include "Concepts/Concepts.h"
#include "Utilities/Singleton/Singleton.h"
#include "Utilities/UUID/UUID.h"

namespace RNGOEngine::AssetHandling
{
    // TODO: This should really be a singleton.
    // TODO: Callback on asset state change? Could be used for hot-reload.
    class AssetDatabase : public Utilities::Singleton<AssetDatabase>
    {
    public:
        AssetDatabase();

        // TODO: Would it be nicer for the user to provide the AssetType and have the database create the correct metadata type internally?
        // Instead of templated functions?
        // Register / Unregister
        template<Concepts::DerivedFrom<AssetMetadata> TMetadata>
        AssetHandle RegisterAsset(const std::filesystem::path& assetPath);

        template<Concepts::DerivedFrom<AssetMetadata> TMetadata>
        void UnregisterAsset(const AssetHandle& uuid);

        // State
    public:
        bool IsRegistered(const AssetHandle& handle) const;

        AssetState GetAssetState(const AssetHandle& handle) const;

        bool IsRegistered(const std::filesystem::path& path) const;

        // Unchecked
        AssetHandle GetAssetHandle(const std::filesystem::path& path) const;

        AssetMetadata& GetAssetMetadata(const AssetHandle& handle);

        const AssetMetadata& GetAssetMetadata(const AssetHandle& handle) const;

        // Checked
        std::optional<AssetHandle> TryGetAssetHandle(const std::filesystem::path& path) const;

        std::optional<std::reference_wrapper<AssetMetadata>> TryGetAssetMetadata(const AssetHandle& handle);

        std::optional<std::reference_wrapper<const AssetMetadata>> TryGetAssetMetadata(
            const AssetHandle& handle) const;

        // Unchecked - Templated
        template<Concepts::DerivedFrom<AssetMetadata> TMetadata>
        TMetadata& GetAssetMetadataAs(const AssetHandle& handle);

        template<Concepts::DerivedFrom<AssetMetadata> TMetadata>
        const TMetadata& GetAssetMetadataAs(const AssetHandle& handle) const;

        // Checked - Templated
        template<Concepts::DerivedFrom<AssetMetadata> TMetadata>
        std::optional<std::reference_wrapper<TMetadata>> TryGetAssetMetadataAs(const AssetHandle& handle);

        template<Concepts::DerivedFrom<AssetMetadata> TMetadata>
        std::optional<std::reference_wrapper<const TMetadata>> TryGetAssetMetadataAs(
            const AssetHandle& handle) const;

    private:
        std::unordered_map<AssetType, std::unique_ptr<AssetMetadataStorage>> m_metadataStorages;
        // TODO: Slight memory waste storing the AssetType again here.
        std::unordered_map<AssetHandle, std::pair<AssetType, size_t>> m_handleToStorageIndex;
        std::unordered_map<std::filesystem::path, AssetHandle> m_pathToHandle;

    private:
        const AssetMetadata* GetMetadataOrNullptr(const AssetHandle& handle) const;
    };

#include "AssetDatabase.tpp"
}