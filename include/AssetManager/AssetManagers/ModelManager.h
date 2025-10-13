//
// Created by Oskar.Norberg on 2025-09-08.
//

#pragma once

#include <filesystem>
#include <limits>
#include <vector>

#include "AssetManager/AssetLoaders/ModelLoader.h"
#include "Renderer/RenderID.h"
#include "Utilities/AssetCache/AssetCache.h"

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

    struct ModelData
    {
        std::vector<Core::Renderer::MeshID> meshIDs;
    };

    enum class ModelCreationError
    {
        None,
        FileNotFound,
        FailedToLoad,
        NoMeshesFound,
        UnsupportedFormat,
    };

    class ModelManager
    {
    public:
        explicit ModelManager(Resources::ResourceManager& resourceManager, bool flipUVs = false);

        std::expected<ModelID, ModelCreationError> CreateModel(const std::filesystem::path& path);
        const ModelData& GetModel(ModelID id) const;

    private:
        bool m_doFlipUVs;
        // TODO: Vector of vector....................
        std::vector<ModelData> m_models;

    private:
        Utilities::AssetCache<std::filesystem::path, ModelID> m_modelCache;

    private:
        Resources::ResourceManager& m_resourceManager;

    private:
        std::expected<ModelLoading::ModelHandle, ModelCreationError> LoadModel(const std::filesystem::path& path) const;
        void UnloadModel(ModelLoading::ModelHandle modelHandle);
    };
}