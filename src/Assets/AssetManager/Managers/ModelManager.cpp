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

    ModelCreationError ModelManager::UploadModel(const AssetHandle& assetHandle,
                                                 const ModelLoading::ModelHandle modelHandle)
    {
        auto runtimeData = UploadModelResources(modelHandle);
        m_models.insert({assetHandle, std::move(runtimeData)});

        // TODO:
        return ModelCreationError::None;
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

    RuntimeModelData ModelManager::UploadModelResources(const ModelLoading::ModelHandle modelHandle)
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