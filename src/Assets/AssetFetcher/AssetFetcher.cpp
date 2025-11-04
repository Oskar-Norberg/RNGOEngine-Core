//
// Created by Oskar.Norberg on 2025-09-03.
//

#include "Assets/AssetFetcher/AssetFetcher.h"

#include "Utilities/RNGOAsserts.h"
#include "Utilities/IO/SimpleFileReader/SimpleFileReader.h"

namespace RNGOEngine::AssetHandling
{
    AssetFetcher::AssetFetcher()
        : Singleton(this)
    {
        using enum AssetType;

        AddAssetPath(None, ENGINE_ASSETS_DIR);

        AddAssetPath(Shader, ENGINE_SHADERS_DIR);
        AddAssetPath(Shader, ENGINE_SHADER_INCLUDE_DIR);

        AddAssetPath(Model, ENGINE_MODELS_DIR);
        AddAssetPath(Texture, ENGINE_TEXTURES_DIR);
    }

    std::optional<std::filesystem::path> AssetFetcher::GetPath(const AssetType type,
                                                                     const std::filesystem::path& path) const
    {
        // First try relative to current working directory.
        if (Utilities::IO::FileExists(path))
        {
            return path;
        }

        // Then inside registered paths
        for (const auto& includeDirectory : m_assetPaths.at(type))
        {
            const std::filesystem::path fullAssetPath = includeDirectory / path;

            if (Utilities::IO::FileExists(fullAssetPath))
            {
                return fullAssetPath;
            }
        }

        return std::nullopt;
    }

    void AssetFetcher::AddAssetPath(const AssetType type, const std::filesystem::path& path)
    {
        using enum AssetType;

        if (type == None)
        {
            // Add to all
            // TODO: Feels like a bit of a hack treating 'None' as 'All'.
            for (auto& [assetType, paths] : m_assetPaths)
            {
                m_assetPaths[type].push_back(path);
            }
        }
        else
        {
            // Add to specific type
            m_assetPaths[type].push_back(path);
        }
    }
}