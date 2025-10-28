//
// Created by Oskar.Norberg on 2025-10-27.
//

#pragma once

#include <expected>
#include <filesystem>
#include <optional>
#include <unordered_map>

#include "AssetHandling/AssetDatabase/AssetState.h"
#include "Utilities/UUID/UUID.h"
#include "Renderer/Handles/TextureHandle.h"
#include "Utilities/Containers/GenerationalVector/GenerationalVector.h"

namespace RNGOEngine::AssetHandling
{
    enum class TextureDatabaseError
    {
        TextureNotFound
    };

    enum class TextureDatabaseState { Unloaded, Loaded, UploadedToGPU };

    struct TextureRecord
    {
        // Metadata
        Utilities::UUID uuid;
        std::filesystem::path path;
        // State
        AssetState state = AssetState::Unregistered;
        // Data
        // TODO: Storing the TextureHandle here means the Database is tied to asset loading. Is this necessary?
        std::optional<Textures::TextureHandle> texture;
    };
    
    class TextureDatabase
    {
    public:
        Utilities::UUID Insert(Textures::TextureHandle textureHandle, const std::filesystem::path& texturePath);
        std::optional<Utilities::UUID> TryGetTextureUUID(const std::filesystem::path& texturePath) const;
        // TODO: Unload textures.

        std::expected<Textures::TextureHandle, TextureDatabaseError> GetTextureData(
            const Utilities::UUID& uuid) const;
        std::expected<Textures::TextureHandle, TextureDatabaseError> GetTextureData(
            const std::filesystem::path& texturePath) const;

        AssetState GetAssetState(const Utilities::UUID& uuid) const;
        void SetAssetState(const Utilities::UUID& uuid, AssetState state);

    private:
        std::unordered_map<std::filesystem::path, Utilities::UUID> m_texturePathToUUIDMap;
        std::unordered_map<Utilities::UUID, Containers::Vectors::GenerationalKey<TextureRecord>> m_textureUUIDToKeyMap;
        Containers::Vectors::GenerationalVector<TextureRecord> m_textureRecords;
    };
}