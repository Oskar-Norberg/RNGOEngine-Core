//
// Created by ringo on 2025-11-02.
//

#include "Assets/AssetImporters/TextureAssetImporter.h"

#include "Assets/AssetDatabase/AssetDatabase.h"
#include "Assets/AssetLoaders/TextureLoader.h"
#include "Assets/AssetManager/AssetManager.h"

namespace RNGOEngine::AssetHandling
{
    ImportingError TextureAssetImporter::LoadFromDisk(
        RuntimeAssetRegistry& registry, const AssetMetadata& metadata
    )
    {
        const auto& typedMetadata = static_cast<const TextureMetadata&>(metadata);

        // Load Texture to RAM
        const auto textureHandle = TextureLoader::LoadTexture(typedMetadata.Path);
        if (!textureHandle)
        {
            switch (textureHandle.error())
            {
                case TextureLoader::TextureLoadingError::FileNotFound:
                    return ImportingError::FileNotFound;
                case TextureLoader::TextureLoadingError::FailedToLoad:
                    return ImportingError::MalformedFile;
                default:
                    return ImportingError::UnknownError;
            }
        }

        // Upload to GPU
        const auto textureDataSpan = std::as_bytes(
            std::span(
                textureHandle.value().data,
                textureHandle.value().width * textureHandle.value().height * textureHandle.value().nrChannels
            )
        );

        // TODO: For now, just get format from nrChannels. Should this be a part of the metadata?

        const Core::Renderer::TextureFormat format = textureHandle->nrChannels == 3
                                                         ? Core::Renderer::TextureFormat::RGB
                                                         : Core::Renderer::TextureFormat::RGBA;

        const Core::Renderer::Texture2DProperties properties{
            .Format = format,
            .MinifyingFilter = typedMetadata.MinifyingFilter,
            .MagnifyingFilter = typedMetadata.MagnifyingFilter,
            .WrappingMode = typedMetadata.WrappingMode,
        };
        auto textureResult = AssetManager::GetInstance().GetTextureManager().UploadTexture(
            typedMetadata.UUID, properties, textureHandle.value().width, textureHandle.value().height,
            std::as_bytes(textureDataSpan)
        );

        if (!textureResult)
        {
            switch (textureResult.error())
            {
                case TextureManagerError::None:
                    break;
                default:
                    return ImportingError::UnknownError;
            }
        }

        // Unload Model from RAM
        TextureLoader::FreeTexture(textureHandle.value());

        auto& entry = registry.Insert<TextureAsset>(metadata.UUID, std::move(textureResult.value()));
        entry.SetState(AssetState::Ready);

        return ImportingError::None;
    }

    ImportingError TextureAssetImporter::FinalizeLoad(
        Data::ThreadType threadType, RuntimeAssetRegistry& registry
    )
    {
        return ImportingError::None;
    }

    void TextureAssetImporter::Unload(const AssetHandle& handle)
    {
        AssetManager::GetInstance().GetTextureManager().UnloadTexture(handle);
    }

    std::unique_ptr<AssetMetadata> TextureAssetImporter::CreateDefaultMetadata(
        const std::filesystem::path& path
    ) const
    {
        auto metadata = std::make_unique<TextureMetadata>();
        metadata->Type = AssetType::Texture;
        return std::move(metadata);
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