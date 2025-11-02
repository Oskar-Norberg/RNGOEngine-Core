//
// Created by ringo on 2025-11-02.
//

#pragma once

#include "AssetImporter.h"

namespace RNGOEngine::AssetHandling
{
    class TextureAssetImporter : public AssetImporter
    {
    public:
        TextureAssetImporter(AssetDatabase& assetDatabase, AssetManager& assetManager)
            : AssetImporter(assetDatabase, assetManager)
        {
        }

        AssetHandle Load(const std::filesystem::path& path) override;
    };
}