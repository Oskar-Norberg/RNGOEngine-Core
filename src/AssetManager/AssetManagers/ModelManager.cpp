//
// Created by Oskar.Norberg on 2025-09-08.
//

#include "AssetManager/AssetManagers/ModelManager.h"

#include "Renderer/IRenderer.h"
#include "ResourceManager/ResourceManager.h"
#include "Utilities/RNGOAsserts.h"

namespace RNGOEngine::AssetHandling
{
    ModelManager::ModelManager(Resources::ResourceManager& resourceManager, bool flipUVs)
        : m_doFlipUVs(flipUVs), m_resourceManager(resourceManager)
    {
    }

    std::expected<ModelID, ModelCreationError> ModelManager::CreateModel(const std::filesystem::path& path)
    {
        // Check cache for model
        if (m_modelCache.Contains(path))
        {
            return m_modelCache.Get(path);
        }

        // Load Model into RAM
        const auto modelHandle = LoadModel(path);
        if (!modelHandle)
        {
            return std::unexpected(modelHandle.error());
        }
        const auto& verifiedModelHandle = modelHandle.value();

        // Upload model to GPU
        ModelData modelData;
        modelData.meshIDs.reserve(verifiedModelHandle.data->meshes.size());

        for (const auto& meshData : verifiedModelHandle.data->meshes)
        {
            modelData.meshIDs.emplace_back(m_resourceManager.CreateMesh(meshData));
        }

        // Unload model from RAM
        UnloadModel(verifiedModelHandle);

        // Cache and return
        m_models.emplace_back(modelData);
        m_modelCache.Insert(path, m_models.size() - 1);

        return m_models.size() - 1;
    }

    std::expected<ModelLoading::ModelHandle, ModelCreationError> ModelManager::LoadModel(
        const std::filesystem::path& path) const
    {
        auto modelHandle = ModelLoading::LoadModel(path, m_doFlipUVs);

        if (!modelHandle)
        {
            // Unholy code
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

    const ModelData& ModelManager::GetModel(const ModelID id) const
    {
        return m_models.at(id);
    }
}