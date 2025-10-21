//
// Created by Oskar.Norberg on 2025-09-08.
//

#include "AssetManager/AssetManagers/ModelManager.h"

#include <unordered_set>

#include "Renderer/IRenderer.h"
#include "ResourceManager/ResourceManager.h"
#include "Utilities/RNGOAsserts.h"

namespace RNGOEngine::AssetHandling
{
    ModelManager::ModelManager(Resources::ResourceManager& resourceManager, bool flipUVs)
        : m_doFlipUVs(flipUVs), m_resourceManager(resourceManager)
    {
    }

    std::expected<Containers::Vectors::GenerationalKey<ModelData>, ModelCreationError> ModelManager::
    CreateModel(const std::filesystem::path& path)
    {
        // Check cache for model
        if (m_modelCache.Contains(path))
        {
            if (m_models.IsValid(m_modelCache.Get(path)))
            {
                return m_modelCache.Get(path);
            }
            else
            {
                m_modelCache.Remove(path);
            }
        }

        // Load Model into RAM
        const auto modelHandle = LoadModel(path);
        if (!modelHandle)
        {
            return std::unexpected(modelHandle.error());
        }

        // Upload model to GPU
        const auto modelData = UploadModel(modelHandle.value());

        // Unload model from RAM
        UnloadModel(modelHandle.value());

        // Cache and return
        const auto key = m_models.Insert(modelData);
        m_modelCache.Insert(path, key);

        return key;
    }

    void ModelManager::UpdateModelCache(ModelData& modelData)
    {
        modelData.CachedMeshes.clear();
        modelData.CachedMeshes.reserve(modelData.meshKeys.size());

        for (const auto& meshKey : modelData.meshKeys)
        {
            const auto meshID = m_resourceManager.GetMeshResource(meshKey);
            if (meshID.has_value())
            {
                modelData.CachedMeshes.emplace_back(meshID.value().get());
            }
            else
            {
                RNGO_ASSERT(false && "ModelManager::UpdateModelCache Mesh key invalid!");
            }
        }
    }

    std::expected<ModelLoading::ModelHandle, ModelCreationError> ModelManager::LoadModel(
        const std::filesystem::path& path) const
    {
        auto modelHandle = ModelLoading::LoadModel(path, m_doFlipUVs);

        if (!modelHandle)
        {
            // Unholy enum-conversion
            switch (modelHandle.error())
            {
                case ModelLoading::ModelLoadingError::None:
                    return std::unexpected(ModelCreationError::FailedToLoad);

                case ModelLoading::ModelLoadingError::FileNotFound:
                    return std::unexpected(ModelCreationError::FileNotFound);

                case ModelLoading::ModelLoadingError::FailedToLoad:
                    return std::unexpected(ModelCreationError::FailedToLoad);

                case ModelLoading::ModelLoadingError::NoMeshesFound:
                    return std::unexpected(ModelCreationError::NoMeshesFound);

                case ModelLoading::ModelLoadingError::UnsupportedFormat:
                    return std::unexpected(ModelCreationError::UnsupportedFormat);

                default:
                    return std::unexpected(ModelCreationError::FailedToLoad);
            }
        }

        return modelHandle.value();
    }

    void ModelManager::UnloadModel(const ModelLoading::ModelHandle modelHandle)
    {
        ModelLoading::UnloadModel(modelHandle);
    }

    ModelData ModelManager::UploadModel(ModelLoading::ModelHandle modelHandle)
    {
        ModelData modelData;
        modelData.meshKeys.reserve(modelHandle.data->meshes.size());

        for (const auto& meshData : modelHandle.data->meshes)
        {
            modelData.meshKeys.emplace_back(m_resourceManager.CreateMesh(meshData));
        }

        UpdateModelCache(modelData);

        return modelData;
    }

    std::span<const Resources::MeshResource> ModelManager::GetModel(
        const Containers::Vectors::GenerationalKey<ModelData>& key) const
    {
        // TODO: This does mean key validity checking in the update loop. Fine for now!
        if (const auto modelDataOpt = m_models.GetValidated(key); modelDataOpt)
        {
            return modelDataOpt.value().get().CachedMeshes;
        }

        if (const auto modelDataOpt = m_models.GetValidated(GetInvalidModel()); modelDataOpt)
        {
            return modelDataOpt.value().get().CachedMeshes;
        }

        return {};
    }

    Containers::Vectors::GenerationalKey<ModelData> ModelManager::GetInvalidModel() const
    {
        // TODO: Return an actual error model.
        return RNGOEngine::Containers::Vectors::GenerationalVector<
            RNGOEngine::AssetHandling::ModelData>::InvalidKey();
    }

    std::span<const Containers::Vectors::GenerationalKey<RNGOEngine::Resources::MeshResource>> ModelManager::
    GetAllMeshKeys(const Containers::Vectors::GenerationalKey<ModelData>& key) const
    {
        // TODO: This does mean key validity checking in the update loop. Fine for now!
        if (const auto modelDataOpt = m_models.GetValidated(key); modelDataOpt)
        {
            return modelDataOpt.value().get().meshKeys;
        }

        if (const auto modelDataOpt = m_models.GetValidated(GetInvalidModel()); modelDataOpt)
        {
            RNGO_ASSERT(false && "ModelManager::GetAllMeshKeys called with invalid model key.");
            return modelDataOpt.value().get().meshKeys;
        }

        return {};
    }

    void ModelManager::RebuildCache()
    {
        for (const auto& modelKey : m_models.Live())
        {
            if (auto modelDataOpt = m_models.GetValidated(modelKey); modelDataOpt)
            {
                UpdateModelCache(modelDataOpt.value().get());
            }
        }
    }

    void ModelManager::OnMeshDestroyed(
        const Containers::Vectors::GenerationalKey<Resources::MeshResource>& meshKey)
    {
        // TODO: This is going to be the slowest thing ever.
        std::unordered_set<Containers::Vectors::GenerationalKey<ModelData>> modelsToUpdate;

        for (const auto& modelKey : m_models.Live())
        {
            if (auto modelDataOpt = m_models.GetValidated(modelKey); modelDataOpt)
            {
                auto& modelData = modelDataOpt.value().get();
                const auto& meshKeys = modelData.meshKeys;

                if (std::ranges::find(meshKeys, meshKey) != meshKeys.end())
                {
                    modelsToUpdate.insert(modelKey);
                }
            }
        }

        for (const auto& modelKey : modelsToUpdate)
        {
            m_models.Remove(modelKey);
        }
    }
}