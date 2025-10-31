//
// Created by Oskar.Norberg on 2025-09-03.
//

#include "Assets/AssetFetcher/AssetFetcher.h"

#include "Utilities/RNGOAsserts.h"
#include "Utilities/IO/SimpleFileReader/SimpleFileReader.h"

namespace RNGOEngine::AssetHandling
{
    AssetFetcher::AssetFetcher()
    {
        AddAssetPath(ENGINE_ASSETS_DIR, AssetPathType::All);

        AddAssetPath(ENGINE_SHADERS_DIR, AssetPathType::Shader);
        AddAssetPath(ENGINE_SHADER_INCLUDE_DIR, AssetPathType::Shader);

        AddAssetPath(ENGINE_MODELS_DIR, AssetPathType::Mesh);
        AddAssetPath(ENGINE_TEXTURES_DIR, AssetPathType::Texture);
    }

    std::optional<std::filesystem::path> AssetFetcher::GetShaderPath(
        const std::filesystem::path& path) const
    {
        return FindFileInPaths(path, m_shaderPaths);
    }

    std::optional<std::filesystem::path> AssetFetcher::GetTexturePath(
        const std::filesystem::path& path) const
    {
        return FindFileInPaths(path, m_texturePaths);
    }

    std::optional<std::filesystem::path> AssetFetcher::GetMeshPath(
        const std::filesystem::path& path) const
    {
        return FindFileInPaths(path, m_meshPaths);
    }

    void AssetFetcher::AddAssetPath(const std::filesystem::path& path, AssetPathType type)
    {
        // TODO: Get feedback on if bringing enums into scope is good or not.
        using enum AssetPathType;

        switch (type)
        {
            case All:
            {
                AddShaderPath(path);
                AddMeshPath(path);
                AddTexturePath(path);
            }
            break;
            case Shader:
                AddShaderPath(path);
                break;
            case Texture:
                AddTexturePath(path);
                break;
            case Mesh:
                AddMeshPath(path);
                break;
            default:
                RNGO_ASSERT(false && "Unsupported asset path type");
        }
    }

    std::optional<std::filesystem::path> AssetFetcher::FindFileInPaths(const std::filesystem::path& path,
                                                                       std::span<const std::filesystem::path>
                                                                       searchPaths) const
    {
        // First try relative to current working directory.
        if (Utilities::IO::FileExists(path))
        {
            return path;
        }

        for (const auto& includeDirectory : searchPaths)
        {
            std::filesystem::path fullAssetPath = includeDirectory / path;

            if (Utilities::IO::FileExists(fullAssetPath))
            {
                return fullAssetPath;
            }
        }

        return std::nullopt;
    }
}