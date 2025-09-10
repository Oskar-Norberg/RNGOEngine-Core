//
// Created by Oskar.Norberg on 2025-09-08.
//

#pragma once

#include <filesystem>
#include <limits>
#include <optional>
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
        ModelID CreateModel(const std::filesystem::path& path, std::vector<Core::Renderer::MeshID> meshes);

        std::optional<ModelID> GetModelIDIfLoaded(const std::filesystem::path& path);
        std::optional<std::reference_wrapper<const ModelData>> GetModel(ModelID id) const;

    private:
        std::unordered_map<ModelID, ModelData> m_models;
        std::unordered_map<std::filesystem::path, ModelID> m_loadedModelPaths;

    private:
        ModelID m_nextModelID = 0;
    };
}