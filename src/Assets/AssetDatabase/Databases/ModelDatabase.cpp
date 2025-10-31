//
// Created by Oskar.Norberg on 2025-10-27.
//

#include "Assets/AssetDatabase/Databases/ModelDatabase.h"

#include "Utilities/RNGOAsserts.h"

namespace RNGOEngine::AssetHandling
{
    Utilities::UUID ModelDatabase::Insert(ModelLoading::ModelHandle modelHandle,
                                          const std::filesystem::path& modelPath)
    {
        const auto uuid = Utilities::UUID();
        ModelRecord record{
            .uuid = uuid,
            .path = modelPath,
            .state = AssetState::Registered,
            .model = modelHandle,
        };

        const auto key = m_modelRecords.Insert(std::move(record));
        m_modelUUIDToIndexMap.insert({uuid, key});
        m_modelPathToUUIDMap.insert({modelPath, uuid});

        return uuid;
    }

    std::optional<Utilities::UUID> ModelDatabase::TryGetModelUUID(
        const std::filesystem::path& modelPath) const
    {
        if (m_modelPathToUUIDMap.contains(modelPath))
        {
            return m_modelPathToUUIDMap.at(modelPath);
        }

        return std::nullopt;
    }

    std::expected<ModelLoading::ModelHandle, ModelDatabaseError> ModelDatabase::GetModelData(
        const Utilities::UUID& uuid) const
    {
        if (m_modelUUIDToIndexMap.contains(uuid))
        {
            const auto key = m_modelUUIDToIndexMap.at(uuid);
            const auto recordOpt = m_modelRecords.GetUnmarkedValidated(key);
            if (recordOpt.has_value())
            {
                return recordOpt->get().model.value();
            }
        }

        return std::unexpected(ModelDatabaseError::ModelNotFound);
    }

    std::expected<ModelLoading::ModelHandle, ModelDatabaseError> ModelDatabase::GetModelData(
        const std::filesystem::path& modelPath) const
    {
        if (m_modelPathToUUIDMap.contains(modelPath))
        {
            const auto uuid = m_modelPathToUUIDMap.at(modelPath);
            return GetModelData(uuid);
        }

        return std::unexpected(ModelDatabaseError::ModelNotFound);
    }

    AssetState ModelDatabase::GetAssetState(const Utilities::UUID& uuid) const
    {
        const auto modelOpt = m_modelRecords.GetUnmarkedValidated(
            m_modelUUIDToIndexMap.at(uuid));

        if (modelOpt)
        {
            return modelOpt->get().state;
        }

        return AssetState::Unregistered;
    }

    void ModelDatabase::SetAssetState(const Utilities::UUID& uuid, AssetState state)
    {
        const auto modelOpt = m_modelRecords.GetUnmarkedValidated(
            m_modelUUIDToIndexMap.at(uuid));

        if (modelOpt)
        {
            modelOpt->get().state = state;
        }
    }
}