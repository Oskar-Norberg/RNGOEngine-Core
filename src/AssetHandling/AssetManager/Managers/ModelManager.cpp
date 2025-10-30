//
// Created by Oskar.Norberg on 2025-09-08.
//

#include "AssetHandling/AssetManager/Managers/ModelManager.h"
#include "ResourceManager/ResourceManager.h"
#include "Utilities/RNGOAsserts.h"

namespace RNGOEngine::AssetHandling
{
    ModelManager::ModelManager(AssetDatabase& assetDatabase, Resources::ResourceManager& resourceManager,
                               const bool flipUVs)
        : m_doFlipUVs(flipUVs),
          m_assetDatabase(assetDatabase),
          m_resourceManager(resourceManager)
    {
    }

    std::expected<AssetHandle, ModelCreationError> ModelManager::CreateModel(
        const std::filesystem::path& path)
    {
        // Check Database
        const auto handleOpt = m_assetDatabase.TryGetModelUUID(path);
        if (handleOpt.has_value())
        {
            return handleOpt.value();
        }

        // Load Model into RAM
        const auto modelHandle = LoadModel(path);
        if (!modelHandle)
        {
            return std::unexpected(modelHandle.error());
        }

        // Upload model to GPU
        auto modelData = UploadModel(modelHandle.value());

        // Unload model from RAM
        UnloadModel(modelHandle.value());

        // Add model to database
        auto assetHandle = m_assetDatabase.Insert(modelHandle.value(), path);
        // Mark Asset as consumed
        m_assetDatabase.SetAssetState(assetHandle, AssetState::Consumed);

        // Add model to runtime data
        m_models.insert({assetHandle, std::move(modelData)});

        return assetHandle;
    }

    std::expected<ModelLoading::ModelHandle, ModelCreationError> ModelManager::LoadModel(
        const std::filesystem::path& path) const
    {
        auto modelHandle = ModelLoading::LoadModel(path, m_doFlipUVs);

        if (!modelHandle)
        {
            // Unholy enum-conversion
            switch (modelHandle.error())
            {
                case ModelLoading::ModelLoadingError::None:
                    return std::unexpected(ModelCreationError::FailedToLoad);

                case ModelLoading::ModelLoadingError::FileNotFound:
                    return std::unexpected(ModelCreationError::FileNotFound);

                case ModelLoading::ModelLoadingError::FailedToLoad:
                    return std::unexpected(ModelCreationError::FailedToLoad);

                case ModelLoading::ModelLoadingError::NoMeshesFound:
                    return std::unexpected(ModelCreationError::NoMeshesFound);

                case ModelLoading::ModelLoadingError::UnsupportedFormat:
                    return std::unexpected(ModelCreationError::UnsupportedFormat);

                default:
                    return std::unexpected(ModelCreationError::FailedToLoad);
            }
        }

        return modelHandle.value();
    }

    void ModelManager::UnloadModel(const ModelLoading::ModelHandle modelHandle)
    {
        ModelLoading::UnloadModel(modelHandle);
    }

    AssetHandle ModelManager::GetInvalidModel() const
    {
        // TODO: Return an actual error model.
        return Utilities::UUID(0);
    }

    const RuntimeModelData& ModelManager::GetRuntimeModelData(const AssetHandle handle) const
    {
        return m_models.at(handle);
    }

    void ModelManager::BeginDestroyAllModels()
    {
        for (const auto& [assetHandle, modelData] : m_models)
        {
            const auto& [meshKeys] = modelData;
            for (const auto& meshKey : meshKeys)
            {
                m_resourceManager.MarkMeshForDestruction(meshKey);
            }
        }
    }

    RuntimeModelData ModelManager::UploadModel(ModelLoading::ModelHandle modelHandle)
    {
        RuntimeModelData modelData;
        modelData.meshKeys.reserve(modelHandle.data->meshes.size());

        for (const auto& meshData : modelHandle.data->meshes)
        {
            modelData.meshKeys.emplace_back(m_resourceManager.CreateMesh(meshData));
        }

        return modelData;
    }
}