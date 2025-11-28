//
// Created by ringo on 2025-11-02.
//

#pragma once

#include <expected>
#include <filesystem>
#include <memory>
#include <span>

#include "Assets/Asset.h"
#include "Assets/RuntimeAssetRegistry/RuntimeAssetRegistry.h"

namespace RNGOEngine
{
    namespace AssetHandling
    {
        class AssetFetcher;
        class AssetDatabase;
        class AssetManager;
    }
}

namespace RNGOEngine::AssetHandling
{
    enum class ImportingError
    {
        None,
        FileNotFound,
        UnsupportedFormat,
        MalformedFile,
        UnknownError,
    };

    class AssetImporter
    {
    public:
        virtual ~AssetImporter() = default;

        virtual ImportingError Load(RuntimeAssetRegistry& registry, const AssetMetadata& metadata) = 0;
        virtual void Unload(const AssetHandle& handle) = 0;

        virtual std::unique_ptr<AssetMetadata> CreateDefaultMetadata(
            const std::filesystem::path& path
        ) const = 0;

    public:
        // TODO: This isn't really being used anywhere right now.
        virtual std::span<const std::string_view> GetSupportedExtensions() const = 0;
    };

    template<typename TAsset>
    class TAssetImporter;

    template<Concepts::DerivedFrom<Asset> TAsset>
    class TAssetImporter<TAsset> : public AssetImporter
    {
    public:
        ImportingError Load(RuntimeAssetRegistry& registry, const AssetMetadata& metadata) override
        {
            auto importResult = ImportAsset(metadata);
            if (!importResult)
            {
                return importResult.error();
            }

            // TODO: Remove this ugly unique ptr garbage
            auto& registryEntry = registry.Insert(
                GetAssetType(), metadata.UUID, std::make_unique<TAsset>(std::move(importResult.value()))
            );
            registryEntry.SetState(AssetState::Ready);

            return ImportingError::None;
        }

        void Unload(const AssetHandle& handle) override = 0;

        std::unique_ptr<AssetMetadata> CreateDefaultMetadata(
            const std::filesystem::path& path
        ) const override = 0;

        std::span<const std::string_view> GetSupportedExtensions() const override = 0;

        // Implementation
    protected:
        virtual std::expected<TAsset, ImportingError> ImportAsset(const AssetMetadata& metadata) = 0;
        // TODO: Bad solution, but works for now.
        virtual AssetType GetAssetType() const = 0;
    };
}