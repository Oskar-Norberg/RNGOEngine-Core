//
// Created by ringo on 2025-11-02.
//

#pragma once

#include <filesystem>

#include "Assets/Asset.h"

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
    class AssetImporter
    {
    public:
        // TODO: Jesus christ just make these singletons already.
        AssetImporter(AssetFetcher& assetFetcher, AssetDatabase& assetDatabase, AssetManager& assetManager)
            : m_assetFetcher(assetFetcher), m_assetDatabase(assetDatabase), m_assetManager(assetManager)
        {
        }

        virtual ~AssetImporter() = default;

    public:
        virtual AssetHandle Load(const std::filesystem::path& path) = 0;

    protected:
        AssetFetcher& m_assetFetcher;
        AssetDatabase& m_assetDatabase;
        AssetManager& m_assetManager;
    };
}