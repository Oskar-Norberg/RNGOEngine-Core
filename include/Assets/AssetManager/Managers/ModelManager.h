//
// Created by Oskar.Norberg on 2025-09-08.
//

#pragma once

#include <vector>

#include "Assets/AssetDatabase/AssetDatabase.h"
#include "Assets/AssetLoaders/ModelLoader.h"
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

        ModelCreationError UploadModel(const AssetHandle& assetHandle, ModelLoading::ModelHandle modelHandle);

    public:
        AssetHandle GetInvalidModel() const;

    public:
        // TODO: Make this return an optional or expected?
        const RuntimeModelData& GetRuntimeModelData(AssetHandle handle) const;

    private:
        Resources::ResourceManager& m_resourceManager;

    private:
        std::unordered_map<AssetHandle, RuntimeModelData> m_models;

    private:
        RuntimeModelData UploadModelResources(ModelLoading::ModelHandle modelHandle);
    };
}