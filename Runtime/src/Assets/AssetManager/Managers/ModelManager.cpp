//
// Created by Oskar.Norberg on 2025-09-08.
//

#include "Assets/AssetManager/Managers/ModelManager.h"

#include "ResourceManager/ResourceManager.h"
#include "Utilities/RNGOAsserts.h"

namespace RNGOEngine::AssetHandling
{
    ModelManager::ModelManager(Resources::ResourceManager& resourceManager)
        : m_resourceManager(resourceManager)
    {
    }
    ModelCreationError ModelManager::UploadModel(
        const AssetHandle& assetHandle, const ModelLoading::ModelData& modelHandle
    )
    {
        auto runtimeData = UploadModelResources(modelHandle);
        m_models.insert({assetHandle, std::move(runtimeData)});

        // TODO:
        return ModelCreationError::None;
    }
    void ModelManager::UnloadModel(const AssetHandle& assetHandle)
    {
        if (m_models.contains(assetHandle))
        {
            const auto& modelData = m_models.at(assetHandle);
            UnloadModelResources(modelData);
            m_models.erase(assetHandle);
        }
    }

    AssetHandle ModelManager::GetInvalidModel() const
    {
        return m_errorModel;
    }

    const RuntimeModelData& ModelManager::GetRuntimeModelData(const AssetHandle& handle) const
    {
        const auto it = m_models.find(handle);
        if (it != m_models.end())
        {
            return it->second;
        }

        RNGO_ASSERT(
            m_models.contains(m_errorModel) &&
            "ModelManager::GetRuntimeModelData - Fallback Error model not loaded!"
        );
        return m_models.at(m_errorModel);
    }

    RuntimeModelData ModelManager::UploadModelResources(const ModelLoading::ModelData& modelData)
    {
        RuntimeModelData runtimeData;
        runtimeData.meshKeys.reserve(modelData.meshes.size());

        for (const auto& meshData : modelData.meshes)
        {
            runtimeData.meshKeys.emplace_back(m_resourceManager.CreateMesh(meshData));
        }

        return runtimeData;
    }

    void ModelManager::UnloadModelResources(const RuntimeModelData& modelData)
    {
        for (const auto& meshKey : modelData.meshKeys)
        {
            m_resourceManager.MarkMeshForDestruction(meshKey);
        }
    }
}