//
// Created by Oskar.Norberg on 2025-09-10.
//

#pragma once

#include <expected>
#include <vector>

#include "AssetManager/AssetLoaders/TextureLoader.h"
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

    struct TextureManagerData
    {
        Containers::Vectors::GenerationalKey<Core::Renderer::TextureID> TextureKey;
        // TODO: Invalidate this on GC
        Core::Renderer::TextureID CachedTextureID;
    };

    class TextureManager
    {
    public:
        explicit TextureManager(Resources::ResourceManager& resourceManager);

        std::expected<Core::Renderer::TextureID, TextureManagerError> CreateTexture(const std::filesystem::path& path);

    public:
        Core::Renderer::TextureID GetTexture(Core::Renderer::TextureID id) const;

    private:
        std::vector<TextureManagerData> m_textures;
        Utilities::AssetCache<std::filesystem::path, Core::Renderer::TextureID> m_textureCache;

    private:
        Resources::ResourceManager& m_resourceManager;

        // Load/Unload to RAM
    private:
        std::expected<Textures::TextureHandle, TextureManagerError> LoadTexture(
            const std::filesystem::path& path);

        void UnloadTexture(Textures::TextureHandle handle);
    };
}