//
// Created by ringo on 2025-11-02.
//

#pragma once

#include "AssetImporter.h"

namespace RNGOEngine::AssetHandling
{
    class ShaderAssetImporter : public AssetImporter
    {
    public:
        ShaderAssetImporter(AssetFetcher& assetFetcher, AssetDatabase& assetDatabase, AssetManager& assetManager);

    public:
        AssetHandle Load(const std::filesystem::path& path) override;
    };
}