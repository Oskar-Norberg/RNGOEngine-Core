//
// Created by ringo on 2025-11-02.
//

#pragma once

#include <filesystem>

#include "Asset.h"
#include "AssetImporters/AssetImporter.h"
#include "AssetSerializers/AssetSerializer.h"
#include "Concepts/Concepts.h"
#include "Utilities/Singleton/Singleton.h"

namespace RNGOEngine::AssetHandling
{
    class AssetFetcher;
    class AssetDatabase;
    class RuntimeAssetRegistry;
}

namespace RNGOEngine::AssetHandling
{
    class AssetLoader : public Utilities::Singleton<AssetLoader>
    {
    public:
        explicit AssetLoader(
            RuntimeAssetRegistry& assetRegistry, AssetDatabase& assetDatabase, AssetFetcher& assetFetcher
        );

        AssetHandle Load(AssetType type, const std::filesystem::path& searchPath) const;

        void LoadPendingAssets(Data::ThreadType threadType) const;

        // Registration
    public:
        // TODO: I don't like these being separate functions, but I cannot be bothered with having a multiple variadic argument function.
        template<Concepts::DerivedFrom<AssetImporter> TAssetImporter, typename... Args>
        void RegisterImporter(const AssetType type, Args&&... args)
        {
            m_importers.insert({type, std::make_unique<TAssetImporter>(std::forward<Args>(args)...)});
        }

        template<Concepts::DerivedFrom<AssetSerializer> TAssetSerializer, typename... Args>
        void RegisterSerializer(const AssetType type, Args&&... args)
        {
            m_serializers[type] = std::make_unique<TAssetSerializer>(std::forward<Args>(args)...);
        }

        template<typename T>
        void UnregisterImporter(const AssetType type)
        {
            m_importers.erase(type);
        }

    private:
        void SaveMetadataToFile(
            const AssetHandle& handle, AssetSerializer& serializer, const std::filesystem::path& metaFilePath
        ) const;

    private:
        RuntimeAssetRegistry& m_assetRegistry;
        AssetDatabase& m_assetDatabase;
        AssetFetcher& m_assetFetcher;

        // TODO: Save these together as a pair
        std::unordered_map<AssetType, std::unique_ptr<AssetImporter>> m_importers;
        std::unordered_map<AssetType, std::unique_ptr<AssetSerializer>> m_serializers;
    };
}