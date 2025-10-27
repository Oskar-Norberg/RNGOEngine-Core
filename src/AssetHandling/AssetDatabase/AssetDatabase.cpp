//
// Created by Oskar.Norberg on 2025-10-24.
//

#include "AssetHandling/AssetDatabase/AssetDatabase.h"

namespace RNGOEngine::AssetHandling
{
    AssetDatabase::AssetDatabase(AssetFetcher& assetFetcher)
        : m_assetFetcher(assetFetcher)
    {
    }

    Utilities::UUID AssetDatabase::LoadModel(const std::filesystem::path& modelPath)
    {
        return m_meshDatabase.LoadModel(modelPath);
    }
}