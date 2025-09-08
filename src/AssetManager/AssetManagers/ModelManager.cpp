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

    const ModelData& ModelManager::GetModel(const ModelID id) const
    {
        if (!m_models.contains(id))
        {
            assert(false && "Model ID does not exist");
            // TODO: Add like an error static model like source engine or something.
            // Undefined behaviour currently
        }

        return m_models.at(id);
    }
}