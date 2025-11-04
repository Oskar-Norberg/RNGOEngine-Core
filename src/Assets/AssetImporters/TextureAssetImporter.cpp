//
// Created by ringo on 2025-11-02.
//

#include "Assets/AssetImporters/TextureAssetImporter.h"

#include "Assets/AssetDatabase/AssetDatabase.h"
#include "Assets/AssetLoaders/TextureLoader.h"
#include "Assets/AssetManager/AssetManager.h"

namespace RNGOEngine::AssetHandling
{
    AssetHandle TextureAssetImporter::Register(const std::filesystem::path& path)
    {
        auto& databaseRef = AssetDatabase::GetInstance();
        // Already in database?
        if (databaseRef.IsRegistered(path))
        {
            // TODO: Check for valid type.
            return databaseRef.GetAssetHandle(path);
        }

        const auto& assetHandle = databaseRef.RegisterAsset<TextureMetadata>(path);
        auto& metadata = databaseRef.GetAssetMetadataAs<TextureMetadata>(assetHandle);
        metadata.State = AssetState::Invalid;
        metadata.Type = AssetType::Texture;
        metadata.Path = path;

        return assetHandle;
    }

    void TextureAssetImporter::Unregister(const AssetHandle& handle)
    {
        auto& databaseRef = AssetDatabase::GetInstance();

        // TODO: Move this to AssetLoader, duplicate code.
        // All types will function the same. Unload if valid, then unregister.

        if (databaseRef.IsRegistered(handle))
        {
            if (const auto& metadata = databaseRef.GetAssetMetadataAs<TextureMetadata>(handle);
                metadata.State == AssetState::Valid)
            {
                Unload(handle);
            }

            databaseRef.UnregisterAsset<TextureMetadata>(handle);
        }
    }

    AssetHandle TextureAssetImporter::Load(const std::filesystem::path& path)
    {
        auto& databaseRef = AssetDatabase::GetInstance();

        const auto& assetHandle = Register(path);
        auto& metadata = databaseRef.GetAssetMetadataAs<TextureMetadata>(assetHandle);

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
            AssetManager::GetInstance().GetTextureManager().UploadTexture(assetHandle, textureHandle.value());
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
        auto& metadata = AssetDatabase::GetInstance().GetAssetMetadataAs<TextureMetadata>(handle);

        if (metadata.State != AssetState::Valid)
        {
            return;
        }

        AssetManager::GetInstance().GetTextureManager().UnloadTexture(handle);
        metadata.State = AssetState::Invalid;
    }
}