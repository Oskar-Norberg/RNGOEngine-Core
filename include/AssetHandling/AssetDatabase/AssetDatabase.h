//
// Created by Oskar.Norberg on 2025-10-24.
//

#pragma once

#include <filesystem>

#include "Databases/ModelDatabase.h"
#include "Utilities/UUID/UUID.h"

namespace RNGOEngine
{
    namespace AssetHandling
    {
        class AssetFetcher;
    }
}

namespace RNGOEngine::AssetHandling
{
    class AssetDatabase
    {
    public:
        explicit AssetDatabase(AssetFetcher& assetFetcher);

    public:
        Utilities::UUID LoadModel(const std::filesystem::path& modelPath);

    private:
        AssetFetcher& m_assetFetcher;

    private:
        ModelDatabase m_modelDatabase;
    };
}