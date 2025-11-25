//
// Created by ringo on 2025-11-02.
//

#include "Assets/AssetLoader.h"

#include <yaml-cpp/yaml.h>

#include <magic_enum/magic_enum.hpp>
#include <ranges>

#include "Assets/AssetDatabase/AssetDatabase.h"
#include "Assets/AssetFetcher/AssetFetcher.h"
#include "Assets/Builtin/BuiltinAssetBootstrapper.h"
#include "Assets/RuntimeAssetRegistry/RuntimeAssetRegistry.h"
#include "Logging/Logger.h"
#include "Utilities/IO/SimpleFileReader/SimpleFileReader.h"
#include "Utilities/RNGOAsserts.h"

namespace RNGOEngine::AssetHandling
{
    AssetLoader::AssetLoader(
        RuntimeAssetRegistry& assetRegistry, AssetDatabase& assetDatabase, AssetFetcher& assetFetcher
    )
        : Singleton(this),
          m_assetRegistry(assetRegistry),
          m_assetDatabase(assetDatabase),
          m_assetFetcher(assetFetcher)

    {
    }

    AssetHandle AssetLoader::Load(const AssetType type, const std::filesystem::path& searchPath) const
    {
        RNGO_LOG(
            Core::LogLevel::Info, "AssetLoader::Load - Loading asset of type {} from path '{}'.",
            magic_enum::enum_name(type), searchPath.string()
        );

        // TODO: This should probably check the iterator against end instead of using contains. But this looks nicer.
        if (!m_serializers.contains(type) || !m_importers.contains(type))
        {
            // TODO: Log error instead of asserting.
            RNGO_ASSERT(false && "AssetLoader::Load - No serializer/importer registered for type.");
            return BuiltinAssets::GetErrorHandle(type);
        }

        // Find valid importer for type and extension
        const auto& serializer = m_serializers.at(type);
        const auto& importer = m_importers.at(type);

        const std::string extension = searchPath.extension().string();

        // Get full path for asset
        const auto fullPath = m_assetFetcher.GetPath(type, searchPath);
        if (!fullPath)
        {
            // TODO: Log failure
            return BuiltinAssets::GetErrorHandle(type);
        }

        auto& database = AssetDatabase::GetInstance();
        auto& registry = RuntimeAssetRegistry::GetInstance();
        // Check if asset has already been loaded
        if (database.IsRegistered(fullPath.value()))
        {
            const auto handle = AssetDatabase::GetInstance().GetAssetHandle(fullPath.value());
            auto& metadata = AssetDatabase::GetInstance().GetAssetMetadata(handle);

            RNGO_ASSERT(metadata.Type == type && "AssetLoader::Load - Asset type mismatch.");
            if (registry.GetState(type, handle) == AssetState::Ready)
            {
                return handle;
            }
        }
        else
        {
            const std::string metaFilePath = fullPath.value().string() + ".meta";
            // Load Metadata if it exists, otherwise create a default and register it.
            if (Utilities::IO::FileExists(metaFilePath))
            {
                YAML::Node node = YAML::LoadFile(metaFilePath);
                std::unique_ptr<AssetMetadata> metadata = serializer->Deserialize(node, fullPath.value());
                AssetDatabase::GetInstance().RegisterAsset(type, std::move(metadata));
            }
            else
            {
                std::unique_ptr<AssetMetadata> metadata = importer->CreateDefaultMetadata(fullPath.value());
                metadata->Path = fullPath.value();
                AssetDatabase::GetInstance().RegisterAsset(type, std::move(metadata));
            }
        }

        // Safe to assume asset is registered.
        auto& metadata = AssetDatabase::GetInstance().GetAssetMetadata(
            AssetDatabase::GetInstance().GetAssetHandle(fullPath.value())
        );

        metadata.Path = fullPath.value();
        auto handle = AssetDatabase::GetInstance().GetAssetHandle(fullPath.value());

        auto importResult = importer->Load(metadata);
        if (!importResult)
        {
            return BuiltinAssets::GetErrorHandle(type);
        }

        auto& registryEntry = m_assetRegistry.Insert(type, handle, std::move(importResult.value()));
        registryEntry.SetState(AssetState::Ready);

        // Save metadata to file?
        // SaveMetadataToFile(handle, *serializer, fullPath.value().string() + ".meta");

        return handle;
    }

    void AssetLoader::SaveMetadataToFile(
        const AssetHandle& handle, AssetSerializer& serializer, const std::filesystem::path& metaFilePath
    ) const
    {
        const auto& metadata = AssetDatabase::GetInstance().GetAssetMetadata(handle);
        YAML::Emitter emitter;
        serializer.Serialize(metadata, emitter);

        std::ofstream metaFile(metaFilePath);
        metaFile << emitter.c_str();
        metaFile.close();
    }
}