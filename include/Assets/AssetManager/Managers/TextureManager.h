//
// Created by Oskar.Norberg on 2025-09-10.
//

#pragma once

#include <expected>

#include "Assets/AssetDatabase/AssetDatabase.h"
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
        // TODO:
    };

    struct RuntimeTextureData
    {
        Containers::GenerationalKey<Core::Renderer::TextureID> TextureKey;
    };

    class TextureManager
    {
    public:
        explicit TextureManager(Resources::ResourceManager& resourceManager);

    public:
        TextureManagerError UploadTexture(const AssetHandle& assetHandle,
                                          Textures::TextureHandle textureHandle);

    public:
        AssetHandle GetInvalidTexture() const;

    public:
        Core::Renderer::TextureID GetTexture(const AssetHandle& uuid) const;

    private:
        std::unordered_map<AssetHandle, RuntimeTextureData> m_textures;

    private:
        Resources::ResourceManager& m_resourceManager;
    };
}