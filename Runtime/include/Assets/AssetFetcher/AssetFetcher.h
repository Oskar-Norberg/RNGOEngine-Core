//
// Created by Oskar.Norberg on 2025-09-03.
//

#pragma once

#include <filesystem>
#include <optional>
#include <vector>

#include "Assets/Asset.h"
#include "Utilities/Singleton/Singleton.h"

namespace RNGOEngine::AssetHandling
{
    class AssetFetcher : public Utilities::Singleton<AssetFetcher>
    {
    public:
        AssetFetcher();

        // Accessors
    public:
        std::optional<std::filesystem::path> GetPath(AssetType type,
                                                           const std::filesystem::path& path) const;
        void AddAssetPath(AssetType type, const std::filesystem::path& path);

    private:
        std::array<std::vector<std::filesystem::path>, AssetTypeCount> m_assetPaths;
    };
}