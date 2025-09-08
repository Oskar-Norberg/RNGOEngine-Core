//
// Created by Oskar.Norberg on 2025-09-08.
//

#pragma once

#include <limits>
#include <unordered_map>
#include <vector>

#include "Renderer/RenderID.h"

// Move to a Manager namespace?
namespace RNGOEngine::AssetHandling
{
    using ModelID = unsigned int;
    constexpr auto INVALID_MODEL_ID = std::numeric_limits<ModelID>::max();

    struct ModelData
    {
        std::vector<Core::Renderer::MeshID> meshes;
    };
    
    class ModelManager
    {
    public:
        ModelID CreateModel(std::vector<Core::Renderer::MeshID> meshes);
        const ModelData& GetModel(ModelID id) const;

    private:
        std::unordered_map<ModelID, ModelData> m_models;

    private:
        ModelID m_nextModelID = 0;
    };
}