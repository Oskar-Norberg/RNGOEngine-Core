//
// Created by ringo on 2025-11-02.
//

#include "Assets/AssetImporters/TextureAssetImporter.h"

#include "Assets/AssetDatabase/AssetDatabase.h"
#include "Assets/AssetLoaders/TextureLoader.h"
#include "Assets/AssetManager/AssetManager.h"

namespace RNGOEngine::AssetHandling
{
    TextureAssetImporter::TextureAssetImporter(
        AssetFetcher& assetFetcher, AssetDatabase& assetDatabase, AssetManager& assetManager
    )
        : AssetImporter(assetFetcher, assetDatabase, assetManager)
    {
    }

    AssetHandle TextureAssetImporter::Register(const std::filesystem::path& path)
    {
        // Already in database?
        if (m_assetDatabase.IsRegistered(path))
        {
            // TODO: Check for valid type.
            return m_assetDatabase.GetAssetHandle(path);
        }

        const auto& assetHandle = m_assetDatabase.RegisterAsset<TextureMetadata>(path);
        auto& metadata = m_assetDatabase.GetAssetMetadataAs<TextureMetadata>(assetHandle);
        metadata.State = AssetState::Invalid;
        metadata.Type = AssetType::Texture;
        metadata.Path = path;

        return assetHandle;
    }

    void TextureAssetImporter::Unregister(const AssetHandle& handle)
    {
        // TODO: Move this to AssetLoader, duplicate code.
        // All types will function the same. Unload if valid, then unregister.

        if (m_assetDatabase.IsRegistered(handle))
        {
            if (const auto& metadata = m_assetDatabase.GetAssetMetadataAs<TextureMetadata>(handle);
                metadata.State == AssetState::Valid)
            {
                Unload(handle);
            }

            m_assetDatabase.UnregisterAsset<TextureMetadata>(handle);
        }
    }

    AssetHandle TextureAssetImporter::Load(const std::filesystem::path& path)
    {
        const auto& assetHandle = Register(path);
        auto& metadata = m_assetDatabase.GetAssetMetadataAs<TextureMetadata>(assetHandle);

        if (metadata.State == AssetState::Valid)
        {
            return assetHandle;
        }

        // Load Texture to RAM
        const auto textureHandle = TextureLoader::LoadTexture(path);
        if (!textureHandle)
        {
            // TODO: Return error texture? Or expected?
            RNGO_ASSERT(false && "TextureAssetImporter::Load - Failed to Load Texture");
        }

        // Upload to GPU
        const auto errorMessage =
            m_assetManager.GetTextureManager().UploadTexture(assetHandle, textureHandle.value());
        if (errorMessage != TextureManagerError::None)
        {
            // TODO: Error handling
            RNGO_ASSERT(false && "TextureAssetImporter::Load - Failed to Load Texture");
        }

        // Unload Model from RAM
        TextureLoader::FreeTexture(textureHandle.value());

        // Mark Valid
        metadata.State = AssetState::Valid;

        return assetHandle;
    }

    void TextureAssetImporter::Unload(const AssetHandle& handle)
    {
        auto& metadata = m_assetDatabase.GetAssetMetadataAs<TextureMetadata>(handle);

        if (metadata.State != AssetState::Valid)
        {
            return;
        }

        m_assetManager.GetTextureManager().UnloadTexture(handle);
        metadata.State = AssetState::Invalid;
    }
}