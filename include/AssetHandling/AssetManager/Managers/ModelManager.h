//
// Created by Oskar.Norberg on 2025-09-08.
//

#pragma once

#include <filesystem>
#include <vector>

#include "AssetHandling/AssetDatabase/AssetDatabase.h"
#include "AssetHandling/AssetLoaders/ModelLoader.h"
#include "ResourceManager/ResourceManager.h"
#include "Utilities/Containers/GenerationalVector/GenerationalVector.h"

namespace RNGOEngine::AssetHandling
{
    class AssetDatabase;
}

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
    struct RuntimeModelData
    {
        std::vector<Containers::Vectors::GenerationalKey<RNGOEngine::Resources::MeshResource>> meshKeys;
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
        explicit ModelManager(AssetDatabase& assetDatabase, Resources::ResourceManager& resourceManager,
                              bool flipUVs = false);

        std::expected<AssetHandle, ModelCreationError> CreateModel(const std::filesystem::path& path);

    public:
        AssetHandle GetInvalidModel() const;

        // Only really relevant for the RenderAPI/ResourceTracker, use friends?
    public:
        // TODO: Make this return an optional or expected?
        const RuntimeModelData& GetRuntimeModelData(AssetHandle handle) const;

        // Engine Internals
    public:
        void BeginDestroyAllModels();

    private:
        bool m_doFlipUVs;
        std::unordered_map<AssetHandle, RuntimeModelData> m_models;

    private:
        AssetDatabase& m_assetDatabase;
        Resources::ResourceManager& m_resourceManager;

        // Model Loading
    private:
        std::expected<ModelLoading::ModelHandle, ModelCreationError> LoadModel(
            const std::filesystem::path& path) const;
        void UnloadModel(ModelLoading::ModelHandle modelHandle);

        // GPU Interfacing
    private:
        RuntimeModelData UploadModel(ModelLoading::ModelHandle modelHandle);
    };
}