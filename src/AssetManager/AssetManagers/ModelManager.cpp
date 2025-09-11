//
// Created by Oskar.Norberg on 2025-09-08.
//

#include "AssetManager/AssetManagers/ModelManager.h"

#include <cassert>

#include "Renderer/IRenderer.h"

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
        m_loadedModelPaths[path] = m_nextModelID;

        return m_nextModelID++;
    }

    std::optional<ModelID> ModelManager::GetModelIDIfLoaded(const std::filesystem::path& path)
    {
        if (const auto it = m_loadedModelPaths.find(path); it != m_loadedModelPaths.end())
        {
            return it->second;
        }

        return std::nullopt;
    }

    std::optional<std::reference_wrapper<const ModelData>> ModelManager::GetModel(const ModelID id) const
    {
        if (const auto it = m_models.find(id); it != m_models.end())
        {
            return std::cref(it->second);
        }

        assert(false && "Model ID not found");
        return std::nullopt;
    }
}