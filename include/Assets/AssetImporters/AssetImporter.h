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
        virtual AssetHandle Register(const std::filesystem::path& path) = 0;
        virtual void Unregister(const AssetHandle& handle) = 0;
        
        virtual AssetHandle Load(const std::filesystem::path& path) = 0;
        virtual void Unload(const AssetHandle& handle) = 0;
        
    protected:
        AssetFetcher& m_assetFetcher;
        AssetDatabase& m_assetDatabase;
        AssetManager& m_assetManager;
    };
}