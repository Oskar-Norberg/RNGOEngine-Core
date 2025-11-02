//
// Created by ringo on 2025-11-02.
//

#pragma once

#include "AssetImporter.h"

namespace RNGOEngine::AssetHandling
{
    class ModelAssetImporter : public AssetImporter
    {
    public:
        // TODO: This ugly construct wouldn't have to be here if they were just singletons...
        ModelAssetImporter(AssetDatabase& assetDatabase, AssetManager& assetManager, const bool doFlipUVs);

        AssetHandle Load(const std::filesystem::path& path) override;

    private:
        bool m_doFlipUVs;
    };
}