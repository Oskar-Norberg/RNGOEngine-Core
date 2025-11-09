//
// Created by ringo on 2025-11-02.
//

#include "Assets/AssetImporters/TextureAssetImporter.h"

#include "Assets/AssetDatabase/AssetDatabase.h"
#include "Assets/AssetLoaders/TextureLoader.h"
#include "Assets/AssetManager/AssetManager.h"

namespace RNGOEngine::AssetHandling
{
    void TextureAssetImporter::Load(const AssetMetadata& metadata)
    {
        auto& typedMetadata = static_cast<const TextureMetadata&>(metadata);

        // Load Texture to RAM
        const auto textureHandle = TextureLoader::LoadTexture(typedMetadata.Path);
        if (!textureHandle)
        {
            // TODO: Return error texture? Or expected?
            RNGO_ASSERT(false && "TextureAssetImporter::Load - Failed to Load Texture");
        }

        // Upload to GPU
        const auto errorMessage =
            AssetManager::GetInstance().GetTextureManager().UploadTexture(
                typedMetadata.UUID, textureHandle.value());
        if (errorMessage != TextureManagerError::None)
        {
            // TODO: Error handling
            RNGO_ASSERT(false && "TextureAssetImporter::Load - Failed to Load Texture");
        }

        // Unload Model from RAM
        TextureLoader::FreeTexture(textureHandle.value());
    }

    void TextureAssetImporter::Unload(const AssetHandle& handle)
    {
        AssetManager::GetInstance().GetTextureManager().UnloadTexture(handle);
    }

    std::unique_ptr<AssetMetadata> TextureAssetImporter::CreateDefaultMetadata(const std::filesystem::path& path) const
    {
        return std::make_unique<TextureMetadata>();
    }

    std::span<const std::string_view> TextureAssetImporter::GetSupportedExtensions() const
    {
        static constexpr std::string_view supportedTypes[] = {
            ".png",
            ".jpg",
            ".jpeg",
        };

        return supportedTypes;
    }
}