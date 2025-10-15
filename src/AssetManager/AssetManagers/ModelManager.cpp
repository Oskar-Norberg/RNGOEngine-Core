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

        // Upload model to GPU
        const auto ModelData = UploadModel(modelHandle.value());

        // Unload model from RAM
        UnloadModel(modelHandle.value());

        // Cache and return
        const auto id = m_models.size();
        m_models.emplace_back(ModelData);
        m_modelCache.Insert(path, id);

        return id;
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

    ModelData ModelManager::UploadModel(ModelLoading::ModelHandle modelHandle)
    {
        ModelData modelData;
        modelData.meshKeys.reserve(modelHandle.data->meshes.size());
        modelData.CachedMeshes.reserve(modelHandle.data->meshes.size());

        for (const auto& meshData : modelHandle.data->meshes)
        {
            modelData.meshKeys.emplace_back(m_resourceManager.CreateMesh(meshData));

            const auto meshResourceOpt = m_resourceManager.GetMeshResource(modelData.meshKeys.back());
            if (meshResourceOpt.has_value())
            {
                modelData.CachedMeshes.emplace_back(meshResourceOpt.value());
            }
        }

        return modelData;
    }

    std::span<const Resources::MeshResource> ModelManager::GetModel(const ModelID id) const
    {
        return m_models.at(id).CachedMeshes;
    }
}