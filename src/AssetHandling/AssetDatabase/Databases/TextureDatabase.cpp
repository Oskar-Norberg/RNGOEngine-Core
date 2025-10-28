//
// Created by Oskar.Norberg on 2025-10-27.
//

#include "AssetHandling/AssetDatabase/Databases/TextureDatabase.h"

namespace RNGOEngine::AssetHandling
{
    Utilities::UUID TextureDatabase::Insert(Textures::TextureHandle textureHandle,
                                            const std::filesystem::path& texturePath)
    {
        const auto uuid = Utilities::UUID();
        TextureRecord record{
            .uuid = uuid,
            .path = texturePath,
            .state = TextureDatabaseState::Loaded,
            .texture = textureHandle,
        };

        const auto key = m_textureRecords.Insert(std::move(record));
        m_textureUUIDToKeyMap.insert({uuid, key});
        m_texturePathToUUIDMap.insert({texturePath, uuid});

        return uuid;
    }

    std::optional<Utilities::UUID> TextureDatabase::TryGetTextureUUID(
        const std::filesystem::path& texturePath) const
    {
        if (m_texturePathToUUIDMap.contains(texturePath))
        {
            return m_texturePathToUUIDMap.at(texturePath);
        }

        return std::nullopt;
    }

    std::expected<Textures::TextureHandle, TextureDatabaseError> TextureDatabase::GetTextureData(
        const Utilities::UUID& uuid) const
    {
        if (m_textureUUIDToKeyMap.contains(uuid))
        {
            const auto textureOpt = m_textureRecords.GetUnmarkedValidated(m_textureUUIDToKeyMap.at(uuid));
            if (textureOpt)
            {
                return textureOpt->get().texture.value();
            }
        }

        return std::unexpected(TextureDatabaseError::TextureNotFound);
    }

    std::expected<Textures::TextureHandle, TextureDatabaseError> TextureDatabase::GetTextureData(
        const std::filesystem::path& texturePath) const
    {
        if (m_texturePathToUUIDMap.contains(texturePath))
        {
            const auto uuid = m_texturePathToUUIDMap.at(texturePath);
            return GetTextureData(uuid);
        }

        return std::unexpected(TextureDatabaseError::TextureNotFound);
    }

    void TextureDatabase::MarkTextureUploaded(const Utilities::UUID uuid)
    {
        const auto key = m_textureUUIDToKeyMap.at(uuid);
        const auto textureOpt = m_textureRecords.GetUnmarkedValidated(key);
        if (!textureOpt)
        {
            RNGO_ASSERT(false && "TextureDatabase::MarkTextureUploaded called with invalid key.");
            return;
        }
        
        auto& textureRecord = textureOpt->get();
        textureRecord.state = TextureDatabaseState::UploadedToGPU;
    }
}