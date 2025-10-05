//
// Created by Oskar.Norberg on 2025-09-08.
//

#pragma once

#include <filesystem>
#include <limits>
#include <unordered_map>
#include <vector>

#include "AssetManager/AssetLoaders/ModelLoader.h"
#include "Renderer/RenderID.h"

namespace RNGOEngine
{
    namespace Resources
    {
        class ResourceManager;
    }
}

// Move to a Manager namespace?
namespace RNGOEngine::AssetHandling
{
    using ModelID = unsigned int;
    constexpr auto INVALID_MODEL_ID = std::numeric_limits<ModelID>::max();

    struct MeshData
    {
        Core::Renderer::MeshID meshID;
        size_t elementCount;
    };

    struct ModelData
    {
        std::vector<MeshData> meshes;
    };

    class ModelManager
    {
    public:
        explicit ModelManager(Resources::ResourceManager& resourceManager);

        ModelID CreateModel(const std::filesystem::path& path, ModelLoading::ModelHandle modelHandle);
        const ModelData& GetModel(ModelID id) const;

    private:
        std::vector<ModelData> m_models;

    private:
        Resources::ResourceManager& m_resourceManager;
        ModelID m_nextModelID = 0;
    };
}