//
// Created by Oskar.Norberg on 2025-09-08.
//

#pragma once

#include <expected>
#include <vector>

#include "Assets/AssetDatabase/AssetDatabase.h"
#include "Assets/AssetLoaders/ModelLoaders/ModelLoaderData.h"
#include "ResourceManager/ResourceManager.h"

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

        std::expected<ModelAsset, ModelCreationError> UploadModel(const AssetHandle& assetHandle, const ModelLoading::ModelData& modelHandle);
        void UnloadModel(const AssetHandle& assetHandle);

    private:
        Resources::ResourceManager& m_resourceManager;

    private:
        // TODO: I hate the fact that we have to store shared_ptrs. But we need to ensure Assets can be devalidated in the database.
        // TODO: This isn't being used????????? Why is this still here????
        std::unordered_map<AssetHandle, std::shared_ptr<ModelAsset>> m_models;

    private:
        MeshCollection UploadModelResources(const ModelLoading::ModelData& modelData);
        void UnloadModelResources(const ModelAsset& modelData);
    };
}