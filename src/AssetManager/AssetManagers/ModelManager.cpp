//
// Created by Oskar.Norberg on 2025-09-08.
//

#include "AssetManager/AssetManagers/ModelManager.h"

#include "Renderer/IRenderer.h"
#include "ResourceManager/ResourceManager.h"
#include "Utilities/RNGOAsserts.h"

namespace RNGOEngine::AssetHandling
{


    ModelManager::ModelManager(Resources::ResourceManager& resourceManager)
        : m_resourceManager(resourceManager)
    {
    }

    ModelID ModelManager::CreateModel(const std::filesystem::path& path,
                                      const ModelLoading::ModelHandle modelHandle)
    {
        ModelData modelData;
        modelData.meshIDs.reserve(modelHandle.data->meshes.size());

        for (const auto& meshData : modelHandle.data->meshes)
        {
            modelData.meshIDs.emplace_back(m_resourceManager.CreateMesh(meshData));
        }

        m_models.emplace_back(modelData);

        return m_nextModelID++;
    }

    const ModelData& ModelManager::GetModel(const ModelID id) const
    {
        return m_models.at(id);
    }
}