//
// Created by Oskar.Norberg on 2025-10-24.
//

#include "AssetHandling/AssetDatabase/AssetDatabase.h"

namespace RNGOEngine::AssetHandling
{
    AssetDatabase::AssetDatabase(AssetFetcher& assetFetcher)
        : m_assetFetcher(assetFetcher),
          m_modelDatabase()
    {
    }

    AssetHandle AssetDatabase::Insert(const ModelLoading::ModelHandle modelHandle,
                                          const std::filesystem::path& modelPath)
    {
        return m_modelDatabase.Insert(modelHandle, modelPath);
    }

    std::optional<AssetHandle> AssetDatabase::TryGetModelUUID(
        const std::filesystem::path& modelPath) const
    {
        return m_modelDatabase.TryGetModelUUID(modelPath);
    }

    std::expected<ModelLoading::ModelHandle, ModelDatabaseError> AssetDatabase::GetModelData(
        const AssetHandle& uuid) const
    {
        return m_modelDatabase.GetModelData(uuid);
    }

    std::expected<ModelLoading::ModelHandle, ModelDatabaseError> AssetDatabase::GetModelData(
        const std::filesystem::path& modelPath) const
    {
        return m_modelDatabase.GetModelData(modelPath);
    }

    // TODO: Clang wants me to pass by ref, look into if that's a good idea.
    void AssetDatabase::MarkModelUploaded(const AssetHandle uuid)
    {
        m_modelDatabase.MarkModelUploaded(uuid);
    }

    Utilities::UUID AssetDatabase::Insert(const Textures::TextureHandle textureHandle,
        const std::filesystem::path& texturePath)
    {
        return m_textureDatabase.Insert(textureHandle, texturePath);
    }

    std::optional<Utilities::UUID> AssetDatabase::TryGetTextureUUID(
        const std::filesystem::path& texturePath) const
    {
        return m_textureDatabase.TryGetTextureUUID(texturePath);
    }

    std::expected<Textures::TextureHandle, TextureDatabaseError> AssetDatabase::GetTextureData(
        const Utilities::UUID& uuid) const
    {
        return m_textureDatabase.GetTextureData(uuid);
    }

    std::expected<Textures::TextureHandle, TextureDatabaseError> AssetDatabase::GetTextureData(
        const std::filesystem::path& texturePath) const
    {
        return m_textureDatabase.GetTextureData(texturePath);
    }

    void AssetDatabase::MarkTextureUploaded(const Utilities::UUID uuid)
    {
        m_textureDatabase.MarkTextureUploaded(uuid);
    }
}