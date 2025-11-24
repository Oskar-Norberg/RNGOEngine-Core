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

    private:
        Resources::ResourceManager& m_resourceManager;

    private:
        std::unordered_map<AssetHandle, ModelAsset> m_models;

    private:
        MeshCollection UploadModelResources(const ModelLoading::ModelData& modelData);
        void UnloadModelResources(const ModelAsset& modelData);
    };
}