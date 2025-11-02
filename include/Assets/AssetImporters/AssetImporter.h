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
        AssetImporter(AssetDatabase& assetDatabase, AssetManager& assetManager)
            : m_assetDatabase(assetDatabase), m_assetManager(assetManager)
        {
        }

        virtual ~AssetImporter() = default;

    public:
        virtual AssetHandle Load(const std::filesystem::path& path) = 0;

    protected:
        AssetDatabase& m_assetDatabase;
        AssetManager& m_assetManager;
    };
}