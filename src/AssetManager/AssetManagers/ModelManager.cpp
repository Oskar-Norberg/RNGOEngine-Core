//
// Created by Oskar.Norberg on 2025-09-08.
//

#include "AssetManager/AssetManagers/ModelManager.h"

#include "Renderer/IRenderer.h"
#include "Utilities/RNGOAsserts.h"

namespace RNGOEngine::AssetHandling
{
    ModelManager::ModelManager(Core::Renderer::IRenderer& renderer)
        : m_renderer(renderer)
    {
    }

    ModelID ModelManager::CreateModel(const std::filesystem::path& path,
                                      ModelLoading::ModelHandle modelHandle)
    {
        std::vector<Core::Renderer::MeshID> meshIDs;
        meshIDs.reserve(modelHandle.data->meshes.size());

        for (const auto& mesh : modelHandle.data->meshes)
        {
            meshIDs.emplace_back(m_renderer.CreateMesh(mesh));
        }

        m_models[m_nextModelID] = ModelData{meshIDs};

        return m_nextModelID++;
    }

    const ModelData& ModelManager::GetModel(const ModelID id) const
    {
        if (const auto it = m_models.find(id); it != m_models.end())
        {
            return it->second;
        }

        // Return a default model?
        RNGO_ASSERT(false && "Tried to get a model that doesn't exist.");
        return m_models.at(0);
    }
}