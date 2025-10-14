//
// Created by Oskar.Norberg on 2025-09-08.
//

#pragma once

#include <filesystem>
#include <limits>
#include <vector>

#include "AssetManager/AssetLoaders/ModelLoader.h"
#include "ResourceManager/ResourceManager.h"
#include "Utilities/AssetCache/AssetCache.h"
#include "Utilities/Containers/GenerationalVector/GenerationalVector.h"

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
        std::vector<Containers::Vectors::GenerationalKey<RNGOEngine::Resources::MeshResource>>
        meshKeys;
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

        // Model Loading
    private:
        std::expected<ModelLoading::ModelHandle, ModelCreationError> LoadModel(
            const std::filesystem::path& path) const;
        void UnloadModel(ModelLoading::ModelHandle modelHandle);

        // GPU Interfacing
    private:
        ModelData UploadModel(ModelLoading::ModelHandle modelHandle);
    };
}