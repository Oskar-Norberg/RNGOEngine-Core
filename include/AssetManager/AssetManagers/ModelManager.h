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
        std::vector<Containers::Vectors::GenerationalKey<RNGOEngine::Resources::MeshResource>> meshKeys;
        // TODO: Invalidate this on GC
        std::vector<Resources::MeshResource> CachedMeshes;
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
        ModelData GetModel(ModelID id) const;

    private:
        bool m_doFlipUVs;
        // TODO: This should probably be a sparse set.
        // TODO: As models are loaded and unloaded this will continue to grow indefinitely and get even more fragmented.
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