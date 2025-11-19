//
// Created by Oskar.Norberg on 2025-09-08.
//

#pragma once

#include <vector>

#include "../../AssetLoaders/ModelLoaders/AssimpModelLoader.h"
#include "Assets/AssetDatabase/AssetDatabase.h"
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
        std::vector<Containers::GenerationalKey<RNGOEngine::Resources::MeshResource>> meshKeys;
    };

    enum class ModelCreationError
    {
        None,
        // TODO:
    };

    class ModelManager
    {
    public:
        explicit ModelManager(Resources::ResourceManager& resourceManager);

        ModelCreationError UploadModel(const AssetHandle& assetHandle, const ModelLoading::ModelData& modelHandle);
        void UnloadModel(const AssetHandle& assetHandle);

    public:
        AssetHandle GetInvalidModel() const;

    public:
        // TODO: Make this return an optional or expected?
        const RuntimeModelData& GetRuntimeModelData(const AssetHandle& handle) const;

        // Engine Internals
    public:
        void SetErrorModel(const AssetHandle& handle)
        {
            m_errorModel = handle;
        }

        AssetHandle GetErrorModel() const
        {
            return m_errorModel;
        }

    private:
        Resources::ResourceManager& m_resourceManager;

    private:
        std::unordered_map<AssetHandle, RuntimeModelData> m_models;
        AssetHandle m_errorModel;

    private:
        RuntimeModelData UploadModelResources(const ModelLoading::ModelData& modelData);
        void UnloadModelResources(const RuntimeModelData& modelData);
    };
}