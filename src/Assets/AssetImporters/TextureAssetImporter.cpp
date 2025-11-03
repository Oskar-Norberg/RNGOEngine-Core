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
    }

    void TextureAssetImporter::Unregister(const AssetHandle& handle)
    {
    }

    AssetHandle TextureAssetImporter::Load(const std::filesystem::path& path)
    {
        // Already in database?
        if (m_assetDatabase.IsRegistered(path))
        {
            // TODO: Reload if invalid?
            return m_assetDatabase.GetAssetHandle(path);
        }

        // Register in Database
        const auto& assetHandle = m_assetDatabase.RegisterAsset<TextureMetadata>(path);

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
        auto& metadata = m_assetDatabase.GetAssetMetadataAs<TextureMetadata>(assetHandle);
        metadata.State = AssetState::Valid;

        return assetHandle;
    }

    void TextureAssetImporter::Unload(const AssetHandle& handle)
    {
    }
}