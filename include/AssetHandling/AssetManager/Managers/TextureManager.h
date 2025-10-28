//
// Created by Oskar.Norberg on 2025-09-10.
//

#pragma once

#include <expected>

#include "AssetHandling/AssetDatabase/AssetDatabase.h"
#include "Renderer/RenderID.h"
#include "Renderer/Handles/TextureHandle.h"
#include "Utilities/AssetCache/AssetCache.h"
#include "Utilities/Containers/GenerationalVector/GenerationalVector.h"

namespace RNGOEngine
{
    namespace Resources
    {
        class ResourceManager;
    }
}

namespace RNGOEngine::AssetHandling
{
    enum class TextureManagerError
    {
        None,
        FileNotFound,
        FailedToLoad,
    };

    struct RuntimeTextureData
    {
        Containers::Vectors::GenerationalKey<Core::Renderer::TextureID> TextureKey;
    };

    class TextureManager
    {
    public:
        explicit TextureManager(AssetDatabase& assetDatabase, Resources::ResourceManager& resourceManager);

    public:
        std::expected<AssetHandle, TextureManagerError> CreateTexture(const std::filesystem::path& path);

    public:
        AssetHandle GetInvalidTexture() const;

    public:
        Core::Renderer::TextureID GetTexture(const AssetHandle& uuid) const;

    private:
        std::unordered_map<AssetHandle, RuntimeTextureData> m_textures;

    private:
        Resources::ResourceManager& m_resourceManager;
        AssetDatabase& m_assetDatabase;

        // Load to RAM
    private:
        std::expected<Textures::TextureHandle, TextureManagerError> LoadFromDisk(
            const std::filesystem::path& path);

        void UnloadTexture(Textures::TextureHandle handle);
    };
}