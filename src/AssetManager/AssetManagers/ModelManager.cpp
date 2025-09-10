//
// Created by Oskar.Norberg on 2025-09-08.
//

#include "AssetManager/AssetManagers/ModelManager.h"

#include <cassert>

namespace RNGOEngine::AssetHandling
{
    ModelID ModelManager::CreateModel(const std::filesystem::path& path,
                                      std::vector<Core::Renderer::MeshID> meshes)
    {
        m_models[m_nextModelID] = ModelData{std::move(meshes)};
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