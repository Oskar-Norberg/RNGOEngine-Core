//
// Created by Oskar.Norberg on 2025-09-03.
//

#pragma once
#include <filesystem>
#include <optional>
#include <vector>

namespace RNGOEngine::AssetHandling
{
    class AssetFileFetcher
    {
    public:
        void AddAssetPath(const std::filesystem::path& path)
        {
            AddShaderPath(path);
            AddTexturePath(path);
            AddMeshPath(path);
        }

    public:
        void AddShaderPath(const std::filesystem::path& path)
        {
            m_shaderPaths.emplace_back(path);
        }

        std::optional<std::filesystem::path> GetShaderPath(const std::filesystem::path& path) const;

    public:
        void AddTexturePath(const std::filesystem::path& path)
        {
            m_texturePaths.emplace_back(path);
        }

        std::optional<std::filesystem::path> GetTexturePath(const std::filesystem::path& path) const;

    public:
        void AddMeshPath(const std::filesystem::path& path)
        {
            m_meshPaths.emplace_back(path);
        }

        std::optional<std::filesystem::path> GetMeshPath(const std::filesystem::path& path) const;

    private:
        std::vector<std::filesystem::path> m_texturePaths;
        std::vector<std::filesystem::path> m_meshPaths;
        std::vector<std::filesystem::path> m_shaderPaths;

    private:
        std::optional<std::filesystem::path> FindFileInPaths(const std::filesystem::path& path,
                                                             const std::vector<std::filesystem::path>&
                                                             searchPaths) const;
    };
}