//
// Created by Oskar.Norberg on 2025-09-03.
//

#pragma once

#include <filesystem>
#include <optional>
#include <vector>

namespace RNGOEngine::AssetHandling
{
    enum class AssetPathType { All, Shader, Texture, Mesh };

    class AssetFetcher
    {
    public:
        AssetFetcher();

        // Accessors
    public:
        std::optional<std::filesystem::path> GetShaderPath(const std::filesystem::path& path) const;
        std::optional<std::filesystem::path> GetTexturePath(const std::filesystem::path& path) const;
        std::optional<std::filesystem::path> GetMeshPath(const std::filesystem::path& path) const;

        // Add Paths
    public:
        void AddAssetPath(const std::filesystem::path& path, AssetPathType type);

    private:
        void AddShaderPath(const std::filesystem::path& path)
        {
            m_shaderPaths.emplace_back(path);
        }

    private:
        void AddTexturePath(const std::filesystem::path& path)
        {
            m_texturePaths.emplace_back(path);
        }

    private:
        void AddMeshPath(const std::filesystem::path& path)
        {
            m_meshPaths.emplace_back(path);
        }

    private:
        std::vector<std::filesystem::path> m_texturePaths;
        std::vector<std::filesystem::path> m_meshPaths;
        std::vector<std::filesystem::path> m_shaderPaths;

    private:
        std::optional<std::filesystem::path> FindFileInPaths(
            const std::filesystem::path& path,
            std::span<const std::filesystem::path>
            searchPaths) const;
    };
}