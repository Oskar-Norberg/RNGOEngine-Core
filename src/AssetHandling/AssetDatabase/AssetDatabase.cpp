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
}