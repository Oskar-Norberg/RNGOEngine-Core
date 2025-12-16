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

    std::optional<AssetHandle> AssetLoader::TryImport(const std::filesystem::path& filePath) const
    {
        const auto importerOpt = GetImporterForExtension(filePath.extension());
        if (!importerOpt)
        {
            RNGO_LOG(
                Core::LogLevel::Warning, "No importer found for file extension '{}'.",
                filePath.extension().string()
            );
            return std::nullopt;
        }

        const auto& importer = importerOpt.value().get();
        const auto type = importer.GetAssetType();

        return Import(type, filePath);
    }

    AssetHandle AssetLoader::Import(const AssetType type, const std::filesystem::path& filePath) const
    {
        // Get full path for asset
        const auto fullPath = m_assetFetcher.GetPath(type, filePath);
        if (!fullPath)
        {
            RNGO_LOG(
                Core::LogLevel::Error, "Failed to find {} at path '{}'.", magic_enum::enum_name(type),
                filePath.string()
            );
            return BuiltinAssets::GetErrorHandle(type);
        }

        auto& database = AssetDatabase::GetInstance();

        // Check if Asset is already registered
        if (database.IsRegistered(fullPath.value()))
        {
            const auto handleOpt = AssetDatabase::GetInstance().TryGetAssetHandle(fullPath.value());
            if (handleOpt)
            {
                return handleOpt.value();
            }
        }

        RNGO_LOG(
            Core::LogLevel::Debug, "Importing {} from path '{}'.", magic_enum::enum_name(type),
            filePath.string()
        );

        const auto& [importer, serializer] = GetImporterAndSerializerOrAssert(type);

        const std::string metaFilePath = fullPath.value().string() + ".meta";

        std::optional<AssetHandle> assetHandle = std::nullopt;

        // Import from Metadata file if it exists
        if (Utilities::IO::FileExists(metaFilePath))
        {
            YAML::Node node = YAML::LoadFile(metaFilePath);
            std::unique_ptr<AssetMetadata> metadata = serializer.Deserialize(node, fullPath.value());
            assetHandle = metadata->UUID;
            // TODO: Ugly absolute path hack. This should be stored relative to project root.
            metadata->Path = absolute(fullPath.value());

            database.RegisterAsset(type, std::move(metadata));
        }
        else
        {
            std::unique_ptr<AssetMetadata> metadata = importer.CreateDefaultMetadata(fullPath.value());
            // TODO: Another ugly absolute path hack
            metadata->Path = absolute(fullPath.value());

            assetHandle = metadata->UUID;
            AssetDatabase::GetInstance().RegisterAsset(type, std::move(metadata));
        }

        RNGO_ASSERT(assetHandle && "AssetLoader::Import - AssetHandle should be valid after import.");

        // Save metadata to file?
        SaveMetadataToFile(assetHandle.value(), serializer, fullPath.value().string() + ".meta");
        return assetHandle.value();
    }

    void AssetLoader::Register(const std::filesystem::path& metadataPath) const
    {
        if (!Utilities::IO::FileExists(metadataPath))
        {
            RNGO_ASSERT(false && "AssetLoader::Register - Metadata file does not exist.");
            RNGO_LOG(Core::LogLevel::Warning, "Metadata file '{}' does not exist.", metadataPath.string());
        }

        YAML::Node node = YAML::LoadFile(metadataPath.string());
        const auto typeOpt = magic_enum::enum_cast<AssetType>(node["Type"].as<std::string>());
        if (!typeOpt)
        {
            RNGO_ASSERT(false && "AssetLoader::Register - Invalid asset type in metadata.");
            RNGO_LOG(
                Core::LogLevel::Error, "Invalid asset type in metadata file '{}'.", metadataPath.string()
            );
            return;
        }

        const auto type = typeOpt.value();
        const auto& [importer, serializer] = GetImporterAndSerializerOrAssert(type);

        const auto nonMetadataPath = metadataPath.parent_path() / metadataPath.stem();
        std::unique_ptr<AssetMetadata> metadata = serializer.Deserialize(node, nonMetadataPath);
        // TODO: Another ugly absolute path hack
        metadata->Path = absolute(nonMetadataPath);

        if (!AssetDatabase::GetInstance().IsRegistered(metadata->UUID))
        {
            AssetDatabase::GetInstance().RegisterAsset(type, std::move(metadata));
            RNGO_LOG(
                Core::LogLevel::Debug, "Registered asset '{}' of type {} from metadata file '{}'.",
                metadataPath.string(), magic_enum::enum_name(type), metadataPath.string()
            );
        }
        else
        {
            RNGO_LOG(Core::LogLevel::Debug, "Asset '{}' is already registered.", metadataPath.string());
        }
    }

    void AssetLoader::Load(const AssetHandle& handle) const
    {
        auto& database = AssetDatabase::GetInstance();
        auto& registry = RuntimeAssetRegistry::GetInstance();

        if (!database.IsRegistered(handle))
        {
            RNGO_LOG(
                Core::LogLevel::Error, "Attempted to load unregistered asset with handle {}.",
                handle.GetValue()
            );

            return;
        }

        const auto& metadata = database.GetAssetMetadata(handle);
        const auto type = metadata.Type;

        const auto& [importer, serializer] = GetImporterAndSerializerOrAssert(type);

        const auto importResult = importer.LoadFromDisk(registry, metadata);
        if (importResult != ImportingError::None)
        {
            RNGO_LOG(
                Core::LogLevel::Error, "Loading {} {} failed with error: {}.", magic_enum::enum_name(type),
                handle.GetValue(), magic_enum::enum_name(importResult)
            );
        }
    }

    void AssetLoader::LoadPendingAssets(Data::ThreadType threadType) const
    {
        for (const auto& [type, importer] : m_importers)
        {
            if (importer->GetFinalizationThreadTypes() & threadType)
            {
                importer->FinalizeLoad(threadType, m_assetRegistry);
            }
        }
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

    std::pair<AssetImporter&, AssetSerializer&> AssetLoader::GetImporterAndSerializerOrAssert(
        const AssetType type
    ) const
    {
        if (!m_serializers.contains(type) || !m_importers.contains(type))
        {
            RNGO_ASSERT(false && "AssetLoader::Load - No serializer/importer registered for type.");
            RNGO_LOG(
                Core::LogLevel::Critical, "No serializer/importer registered for asset type {}.",
                magic_enum::enum_name(type)
            );
            // UBO land if asserts are disabled.
        }

        // Find valid importer for type and extension
        const auto& serializer = m_serializers.at(type);
        const auto& importer = m_importers.at(type);

        return std::make_pair(std::ref(*importer), std::ref(*serializer));
    }

    std::optional<std::reference_wrapper<AssetImporter>> AssetLoader::GetImporterForExtension(
        const std::filesystem::path& extension
    ) const
    {
        for (const auto& [type, importer] : m_importers)
        {
            const auto& supportedExtensions = importer->GetSupportedExtensions();
            if (std::ranges::find(supportedExtensions, extension.string()) != supportedExtensions.end())
            {
                return std::ref(*importer);
            }
        }

        return std::nullopt;
    }
}