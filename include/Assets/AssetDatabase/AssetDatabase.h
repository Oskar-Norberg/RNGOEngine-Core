//
// Created by Oskar.Norberg on 2025-10-24.
//

#pragma once

#include <filesystem>

#include "Assets/Asset.h"
#include  "Assets/AssetMetadataTypes.h"
#include "Concepts/Concepts.h"
#include "Utilities/UUID/UUID.h"

namespace RNGOEngine::AssetHandling
{
    // TODO: This should really be a singleton.
    class AssetDatabase
    {
    public:
        // Register / Unregister
        // TODO: I don't like how the consumer has to specify the type and the metadata type.
        template<Concepts::DerivedFrom<AssetMetadata> TMetadata>
        AssetHandle RegisterAsset(const std::filesystem::path& assetPath);

        template<Concepts::DerivedFrom<AssetMetadata> TMetadata>
        void UnregisterAsset(const AssetHandle& uuid);

        // State
    public:
        bool IsRegistered(const AssetHandle& handle) const;
        AssetState GetAssetState(const AssetHandle& handle) const;

        bool IsRegistered(const std::filesystem::path& path) const;
        AssetHandle GetAssetHandle(const std::filesystem::path& path) const;

        AssetMetadata& GetAssetMetadata(const AssetHandle& handle);
        // TODO: Make a const version and add a try-get version wrapped in std::optional
        // TODO: Consumer has to cast AssetMetadata, implement templated version.


    private:
        // TODO: These uniqueptrs NEED to be stored contiguously based on their type. This is going to perform terribly.
        std::unordered_map<AssetHandle, std::unique_ptr<AssetMetadata>> m_assetMetadataMap;
        std::unordered_map<std::filesystem::path, AssetHandle> m_pathToHandle;
    };

#include "AssetDatabase.tpp"
}