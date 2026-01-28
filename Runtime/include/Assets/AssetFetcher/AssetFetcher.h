//
// Created by Oskar.Norberg on 2025-09-03.
//

#pragma once

#include <filesystem>
#include <optional>
#include <string_view>
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
        std::optional<std::filesystem::path> GetPath(const std::filesystem::path& path) const;
        void AddAssetPath(const std::filesystem::path& path);

    public:
        // Recursively scan all asset paths and callback for each file with said extension.
        void ForEachOfExtension(
            std::string_view extension, const std::function<void(const std::filesystem::path&)>& callback
        );

    private:
        std::vector<std::filesystem::path> m_assetPaths;
    };
}