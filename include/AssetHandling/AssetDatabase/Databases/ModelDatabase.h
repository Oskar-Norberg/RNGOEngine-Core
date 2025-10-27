//
// Created by Oskar.Norberg on 2025-10-27.
//

#pragma once

#include <filesystem>
#include <expected>
#include <optional>
#include <unordered_map>

#include "AssetHandling/AssetLoaders/ModelLoader.h"
#include "Utilities/Containers/GenerationalVector/GenerationalVector.h"
#include "Utilities/UUID/UUID.h"

namespace RNGOEngine::AssetHandling
{
    enum class ModelDatabaseState { Unloaded, Loaded, UploadedToGPU };

    struct ModelRecord
    {
        // Metadata
        Utilities::UUID uuid;
        std::filesystem::path path;
        // State
        ModelDatabaseState state = ModelDatabaseState::Unloaded;
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

        std::expected<ModelLoading::ModelHandle, ModelDatabaseError> GetModelData(
            const Utilities::UUID& uuid) const;
        std::expected<ModelLoading::ModelHandle, ModelDatabaseError> GetModelData(
            const std::filesystem::path& modelPath) const;

        void MarkModelUploaded(Utilities::UUID uuid);

    private:
        Containers::Vectors::GenerationalVector<ModelRecord> m_modelRecords;
        std::unordered_map<std::filesystem::path, Utilities::UUID> m_modelPathToUUIDMap;
        std::unordered_map<Utilities::UUID, Containers::Vectors::GenerationalKey<ModelRecord>>
        m_modelUUIDToIndexMap;
    };
}