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

    std::expected<std::weak_ptr<ModelAsset>, ModelCreationError> ModelManager::UploadModel(
        const AssetHandle& assetHandle, const ModelLoading::ModelData& modelHandle
    )
    {
        auto modelAsset = std::make_shared<ModelAsset>(
            UploadModelResources(modelHandle),
            AssetHandle(assetHandle)
        );
        const auto [it, inserted] = m_models.insert({assetHandle, std::move(modelAsset)});

        // TODO: Error handling
        return it->second;
    }

    void ModelManager::UnloadModel(const AssetHandle& assetHandle)
    {
        if (m_models.contains(assetHandle))
        {
            const auto& modelData = m_models.at(assetHandle);
            UnloadModelResources(*modelData.get());
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
        // TODO:
    }
}