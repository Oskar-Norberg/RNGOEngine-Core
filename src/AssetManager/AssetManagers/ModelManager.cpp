//
// Created by Oskar.Norberg on 2025-09-08.
//

#include "AssetManager/AssetManagers/ModelManager.h"

#include <cassert>

namespace RNGOEngine::AssetHandling
{
    ModelID ModelManager::CreateModel(std::vector<Core::Renderer::MeshID> meshes)
    {
        m_models[m_nextModelID] = ModelData{std::move(meshes)};
        return m_nextModelID++;
    }

    std::optional<std::reference_wrapper<const ModelData>> ModelManager::GetModel(const ModelID id) const
    {
        if (const auto it = m_models.find(id); it != m_models.end()) {
            return std::cref(it->second);
        }

        assert(false && "Model ID not found");
        return std::nullopt;
    }
}