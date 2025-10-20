//
// Created by Oskar.Norberg on 2025-09-10.
//

#pragma once

#include <expected>

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

        std::expected<Containers::Vectors::GenerationalKey<TextureManagerData>, TextureManagerError>
        CreateTexture(const std::filesystem::path& path);

    public:
        Core::Renderer::TextureID GetTexture(Containers::Vectors::GenerationalKey<TextureManagerData> key) const;

    public:
        Containers::Vectors::GenerationalKey<TextureManagerData> GetInvalidTexture() const;

    private:
        Containers::Vectors::GenerationalVector<TextureManagerData> m_textures;
        Utilities::AssetCache<std::filesystem::path, Containers::Vectors::GenerationalKey<TextureManagerData>> m_textureCache;

    private:
        Resources::ResourceManager& m_resourceManager;

        // Load/Unload to RAM
    private:
        std::expected<Textures::TextureHandle, TextureManagerError> LoadTexture(
            const std::filesystem::path& path);

        void UnloadTexture(Textures::TextureHandle handle);
    };
}