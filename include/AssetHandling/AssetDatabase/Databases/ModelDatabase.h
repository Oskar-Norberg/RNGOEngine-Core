//
// Created by Oskar.Norberg on 2025-10-27.
//

#pragma once

#include <filesystem>
#include <expected>
#include <optional>
#include <unordered_map>

#include "AssetHandling/AssetDatabase/AssetState.h"
#include "AssetHandling/AssetLoaders/ModelLoader.h"
#include "Utilities/Containers/GenerationalVector/GenerationalVector.h"
#include "Utilities/UUID/UUID.h"

namespace RNGOEngine::AssetHandling
{
    struct ModelRecord
    {
        // Metadata
        Utilities::UUID uuid;
        std::filesystem::path path;
        // State
        AssetState state = AssetState::Unregistered;
        // Data
        std::optional<ModelLoading::ModelHandle> model;
    };

    enum class ModelDatabaseError
    {
        ModelNotFound
    };

    class ModelDatabase
    {
    public:
        ModelDatabase()
            : m_modelRecords(),
              m_modelPathToUUIDMap(),
              m_modelUUIDToIndexMap()
        {
        }

        Utilities::UUID Insert(ModelLoading::ModelHandle modelHandle, const std::filesystem::path& modelPath);
        std::optional<Utilities::UUID> TryGetModelUUID(const std::filesystem::path& modelPath) const;
        // TODO: Unload models.

        // TODO: Don't store runtime data here.
        std::expected<ModelLoading::ModelHandle, ModelDatabaseError> GetModelData(
            const Utilities::UUID& uuid) const;
        std::expected<ModelLoading::ModelHandle, ModelDatabaseError> GetModelData(
            const std::filesystem::path& modelPath) const;

        AssetState GetAssetState(const Utilities::UUID& uuid) const;
        void SetAssetState(const Utilities::UUID& uuid, AssetState state);

    private:
        Containers::GenerationalVector<ModelRecord> m_modelRecords;
        std::unordered_map<std::filesystem::path, Utilities::UUID> m_modelPathToUUIDMap;
        std::unordered_map<Utilities::UUID, Containers::GenerationalKey<ModelRecord>>
        m_modelUUIDToIndexMap;
    };
}