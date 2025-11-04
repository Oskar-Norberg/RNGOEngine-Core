//
// Created by ringo on 2025-11-02.
//

#pragma once

#include "AssetImporter.h"
#include "Assets/AssetLoaders/ShaderLoader.h"

namespace RNGOEngine::AssetHandling
{
    class ShaderAssetImporter : public AssetImporter
    {
    public:
        AssetHandle Register(const std::filesystem::path& path) override;
        void Unregister(const AssetHandle& handle) override;
        AssetHandle Load(const std::filesystem::path& path) override;
        void Unload(const AssetHandle& handle) override;

    private:
        ShaderLoader m_shaderLoader;
    };
}