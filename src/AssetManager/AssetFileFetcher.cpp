//
// Created by Oskar.Norberg on 2025-09-03.
//

#include "AssetManager/AssetFileFetcher.h"
#include "Utilities/IO/SimpleFileReader/SimpleFileReader.h"

namespace RNGOEngine::AssetHandling
{
    std::optional<std::filesystem::path> AssetFileFetcher::GetShaderPath(
        const std::filesystem::path& path) const
    {
        return FindFileInPaths(path, m_shaderPaths);
    }

    std::optional<std::filesystem::path> AssetFileFetcher::GetTexturePath(
        const std::filesystem::path& path) const
    {
        return FindFileInPaths(path, m_texturePaths);
    }

    std::optional<std::filesystem::path> AssetFileFetcher::GetMeshPath(
        const std::filesystem::path& path) const
    {
        return FindFileInPaths(path, m_meshPaths);
    }

    std::optional<std::filesystem::path> AssetFileFetcher::FindFileInPaths(const std::filesystem::path& path,
        const std::vector<std::filesystem::path>& searchPaths) const
    {
        // First try relative to current working directory.
        if (Utilities::IO::FileExists(path))
        {
            return path;
        }

        for (const auto& includeDirectory : searchPaths)
        {
            // TODO: Look into using a joined view instead of copying the string.
            const std::filesystem::path fullAssetPath = includeDirectory / path;

            if (Utilities::IO::FileExists(fullAssetPath))
            {
                return fullAssetPath;
            }
        }

        return std::nullopt;
    }
}