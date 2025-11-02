//
// Created by ringo on 2025-11-02.
//

#include "Assets/AssetImporters/ShaderAssetImporter.h"

namespace RNGOEngine::AssetHandling
{
    ShaderAssetImporter::ShaderAssetImporter(AssetFetcher& assetFetcher, AssetDatabase& assetDatabase,
                                             AssetManager& assetManager)
        : AssetImporter(assetFetcher, assetDatabase, assetManager)
    {
    }

    AssetHandle ShaderAssetImporter::Load(const std::filesystem::path& path)
    {
        // TODO:
        return Utilities::UUID();
    }
}