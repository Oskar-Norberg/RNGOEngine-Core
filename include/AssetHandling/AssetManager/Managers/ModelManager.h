//
// Created by Oskar.Norberg on 2025-09-08.
//

#pragma once

#include <filesystem>
#include <limits>
#include <vector>

#include "AssetHandling/AssetLoaders/ModelLoader.h"
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
    struct ModelData
    {
        std::vector<Containers::Vectors::GenerationalKey<RNGOEngine::Resources::MeshResource>> meshKeys;
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

        std::expected<Containers::Vectors::GenerationalKey<ModelData>, ModelCreationError> CreateModel(
            const std::filesystem::path& path);
        std::span<const Resources::MeshResource> GetModel(
            const Containers::Vectors::GenerationalKey<ModelData>& key) const;

    public:
        Containers::Vectors::GenerationalKey<ModelData> GetInvalidModel() const;

        // Only really relevant for the RenderAPI/ResourceTracker, use friends?
    public:
        std::span<const Containers::Vectors::GenerationalKey<RNGOEngine::Resources::MeshResource>>
        GetAllMeshKeys(
            const Containers::Vectors::GenerationalKey<ModelData>& key) const;

        // Engine Internal
    public:
        void RebuildCache();

    private:
        bool m_doFlipUVs;
        Containers::Vectors::GenerationalVector<ModelData> m_models;

    private:
        Utilities::AssetCache<std::filesystem::path, Containers::Vectors::GenerationalKey<ModelData>>
        m_modelCache;

    private:
        Resources::ResourceManager& m_resourceManager;

    private:
        void UpdateModelCache(const Containers::Vectors::GenerationalKey<ModelData>& key);

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