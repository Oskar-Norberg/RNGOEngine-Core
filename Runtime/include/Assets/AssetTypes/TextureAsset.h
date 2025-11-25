//
// Created by ringo on 2025-10-31.
//

#pragma once
#include "Assets/Asset.h"

namespace RNGOEngine::AssetHandling
{
    using TextureKey = Containers::GenerationalKey<Core::Renderer::TextureID>;

    class TextureAsset : public Asset
    {
    public:
        TextureAsset(AssetHandle&& handle, TextureKey&& m_texture_key)
            : Asset(std::move(handle)), m_textureKey(std::move(m_texture_key))
        {
        }

        TextureKey GetTextureKey() const
        {
            return m_textureKey;
        }

    private:
        TextureKey m_textureKey;
    };

    // TODO: This is strictly a 2D texture for now.
    struct TextureMetadata : AssetMetadata
    {
        Core::Renderer::TextureFormat Format;
        Core::Renderer::TextureFiltering MinifyingFilter;
        Core::Renderer::TextureFiltering MagnifyingFilter;
        Core::Renderer::TextureWrapping WrappingMode;
    };
}