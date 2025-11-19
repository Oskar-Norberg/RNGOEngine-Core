//
// Created by ringo on 2025-11-02.
//

#pragma once

#include <filesystem>
#include <span>

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
        virtual ~AssetImporter() = default;

        // Should load be able to return failure? On malformed files? Just a bool?
        virtual void Load(const AssetMetadata& metadata) = 0;
        virtual void Unload(const AssetHandle& handle) = 0;

        virtual std::unique_ptr<AssetMetadata> CreateDefaultMetadata(const std::filesystem::path& path) const = 0;

    public:
        // TODO: This isn't really being used anywhere right now.
        virtual std::span<const std::string_view> GetSupportedExtensions() const = 0;
    };
}