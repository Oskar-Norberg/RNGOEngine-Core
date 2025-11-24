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

    std::expected<ModelAsset*, ModelCreationError> ModelManager::UploadModel(
        const AssetHandle& assetHandle, const ModelLoading::ModelData& modelHandle
    )
    {
        ModelAsset modelAsset(UploadModelResources(modelHandle), AssetHandle(assetHandle));
        const auto [it, inserted] = m_models.insert({assetHandle, modelAsset});

        // TODO: Error handling
        return &it->second;
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

    MeshCollection ModelManager::UploadModelResources(const ModelLoading::ModelData& modelData)
    {
        auto& meshResourceManager = m_resourceManager.GetMeshResourceManager();

        MeshCollection meshKeys;
        meshKeys.reserve(modelData.meshes.size());

        for (const auto& meshData : modelData.meshes)
        {
            meshKeys.emplace_back(meshResourceManager.CreateMesh(meshData));
        }

        return meshKeys;
    }

    void ModelManager::UnloadModelResources(const ModelAsset& modelData)
    {
    }

    // const RuntimeModelData& ModelManager::GetRuntimeModelData(const AssetHandle& handle) const
    // {
    //     const auto it = m_models.find(handle);
    //     if (it != m_models.end())
    //     {
    //         return it->second;
    //     }
    //
    //     // TODO:
    //     RNGO_ASSERT(false && "ModelManager::GetRuntimeModelData - TODO: This needs to return an opt");
    //     return {};
    // }
    //
    // RuntimeModelData ModelManager::UploadModelResources(const ModelLoading::ModelData& modelData)
    // {
    //     auto& meshResourceManager = m_resourceManager.GetMeshResourceManager();
    //
    //     RuntimeModelData runtimeData;
    //     runtimeData.meshKeys.reserve(modelData.meshes.size());
    //
    //     for (const auto& meshData : modelData.meshes)
    //     {
    //         runtimeData.meshKeys.emplace_back(meshResourceManager.CreateMesh(meshData));
    //     }
    //
    //     return runtimeData;
    // }
}