//
// Created by ringo on 2025-11-02.
//

#include "Assets/AssetLoader.h"

#include <ranges>

#include <yaml-cpp/yaml.h>

#include "Assets/AssetDatabase/AssetDatabase.h"
#include "Utilities/RNGOAsserts.h"
#include "Utilities/IO/SimpleFileReader/SimpleFileReader.h"

namespace RNGOEngine::AssetHandling
{
    AssetLoader::AssetLoader(AssetDatabase& assetDatabase, AssetFetcher& assetFetcher)
        : m_assetDatabase(assetDatabase), m_assetFetcher(assetFetcher)
    {
    }

    AssetHandle AssetLoader::Load(const AssetType type, const std::filesystem::path& path) const
    {
        RNGO_ASSERT(m_loaders.contains(type) && "No loader registered for the specified asset type.");

        const auto fullPath = m_assetFetcher.GetPath(type, path);
        if (!fullPath)
        {
            RNGO_ASSERT(false && "AssetLoader::Load - Asset not found!");
            // TODO: What to return here?
            // TODO: UB
        }

        // TODO: Add support for multiple importers per asset type.
        // Ensure Importer supports extension.
        // TODO: Lowercase extension? Will allocate a bit of memory.
        // const std::string_view extension = path.extension().string();
        // const auto supportedExtensions = m_loaders.at(type)->GetSupportedExtensions();
        // if (std::ranges::find(supportedExtensions, extension) == supportedExtensions.end())
        // {
        //     RNGO_ASSERT(
        //         false && "AssetLoader::Load - Unsupported file extension for the specified asset type.");
        // }

        // Check for existing metadata.
        const auto& serializer = m_serializers.at(type);
        RNGO_ASSERT(serializer && "AssetLoader::Load No serializer for the specified asset type.");
        const std::string metaFilePath = fullPath->string() + ".meta";
        if (Utilities::IO::FileExists(metaFilePath))
        {
            // Try to load existing metadata.
            YAML::Node node = YAML::LoadFile(metaFilePath.data());
            const AssetHandle handle = m_serializers.at(type)->Deserialize(node, fullPath.value());
        }

        const auto handle = m_loaders.at(type)->Load(fullPath.value());
        // Update or create new metadata
        SaveMetadataToFile(handle, *serializer, metaFilePath);

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