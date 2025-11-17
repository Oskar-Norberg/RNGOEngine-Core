//
// Created by ringo on 2025-11-02.
//

#include "Assets/AssetLoader.h"

#include <yaml-cpp/yaml.h>

#include <ranges>

#include "Assets/AssetDatabase/AssetDatabase.h"
#include "Assets/Builtin/BuiltinAssetBootstrapper.h"
#include "Utilities/IO/SimpleFileReader/SimpleFileReader.h"
#include "Utilities/RNGOAsserts.h"

namespace RNGOEngine::AssetHandling
{
    // TODO: Annoying clang warning about initialization order. But it looks fine??
    AssetLoader::AssetLoader(AssetDatabase& assetDatabase, AssetFetcher& assetFetcher)
        : Singleton(this),
          m_assetDatabase(assetDatabase),
          m_assetFetcher(assetFetcher)
    {
    }

    AssetHandle AssetLoader::Load(const AssetType type, const std::filesystem::path& searchPath) const
    {
        const auto& serializer = m_serializers.at(type);
        const auto& importer = m_loaders.at(type);

        RNGO_ASSERT(
            serializer && importer && "AssetLoader::Load - No serializer/importer registered for type.");

        const auto fullPath = m_assetFetcher.GetPath(type, searchPath);
        if (!fullPath)
        {
            // TODO: Log failure
            return BuiltinAssets::GetErrorHandle(type);
        }

        if (AssetDatabase::GetInstance().IsRegistered(fullPath.value()))
        {
            const auto handle = AssetDatabase::GetInstance().GetAssetHandle(fullPath.value());
            auto& metadata = AssetDatabase::GetInstance().GetAssetMetadata(handle);

            RNGO_ASSERT(metadata.Type == type && "AssetLoader::Load - Asset type mismatch.");
            if (metadata.State == AssetState::Valid)
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
        const auto handle = AssetDatabase::GetInstance().GetAssetHandle(fullPath.value());
        importer->Load(metadata);
        metadata.State = AssetState::Valid;

        // Save metadata to file?
        // SaveMetadataToFile(handle, *serializer, fullPath.value().string() + ".meta");

        return handle;
    }

    void AssetLoader::SaveMetadataToFile(const AssetHandle& handle, AssetSerializer& serializer,
                                         const std::filesystem::path& metaFilePath) const
    {
        const auto& metadata = AssetDatabase::GetInstance().GetAssetMetadata(handle);
        YAML::Emitter emitter;
        serializer.Serialize(metadata, emitter);

        std::ofstream metaFile(metaFilePath);
        metaFile << emitter.c_str();
        metaFile.close();
    }
}