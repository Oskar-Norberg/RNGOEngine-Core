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

    public:
        virtual AssetHandle Register(const std::filesystem::path& path) = 0;
        virtual void Unregister(const AssetHandle& handle) = 0;
        
        virtual AssetHandle Load(const std::filesystem::path& path) = 0;
        virtual void Unload(const AssetHandle& handle) = 0;

    public:
        virtual std::span<const std::string_view> GetSupportedExtensions() const = 0;
    };
}