//
// Created by Oskar.Norberg on 2025-09-08.
//

#include "AssetHandling/AssetManager/Managers/ModelManager.h"

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
        if (const auto cachedModel = m_modelCache.TryGet(path); cachedModel)
        {
            if (m_models.IsValidUnmarked(cachedModel.value()))
            {
                return cachedModel.value();
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

        // Update Model Cache
        UpdateModelCache(key);

        return key;
    }

    void ModelManager::UpdateModelCache(const Containers::Vectors::GenerationalKey<ModelData>& key)
    {
        const auto modelDataOpt = m_models.GetUnmarkedValidated(key);
        if (!modelDataOpt)
        {
            RNGO_ASSERT(false && "ModelManager::UpdateModelCache called with invalid model key.");
            return;
        }

        auto& [meshKeys, CachedMeshes] = modelDataOpt.value().get();
        CachedMeshes.clear();
        CachedMeshes.reserve(meshKeys.size());

        bool invalid = false;
        for (const auto& meshKey : meshKeys)
        {
            const auto meshID = m_resourceManager.GetMeshResource(meshKey);
            if (meshID.has_value())
            {
                CachedMeshes.emplace_back(meshID.value().get());
            }
            else
            {
                // Model has invalid mesh key, has been unloaded by ResourceManager.
                invalid = true;
                break;
            }
        }

        if (invalid)
        {
            m_models.Remove(key);
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
        
        return modelData;
    }

    std::span<const Resources::MeshResource> ModelManager::GetModel(
        const Containers::Vectors::GenerationalKey<ModelData>& key) const
    {
        // TODO: This does mean key validity checking in the update loop. Fine for now!
        if (const auto modelDataOpt = m_models.GetUnmarkedValidated(key); modelDataOpt)
        {
            return modelDataOpt.value().get().CachedMeshes;
        }

        if (const auto modelDataOpt = m_models.GetUnmarkedValidated(GetInvalidModel()); modelDataOpt)
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
        if (const auto modelDataOpt = m_models.GetUnmarkedValidated(key); modelDataOpt)
        {
            return modelDataOpt.value().get().meshKeys;
        }

        if (const auto modelDataOpt = m_models.GetUnmarkedValidated(GetInvalidModel()); modelDataOpt)
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
            UpdateModelCache(modelKey);
        }
    }
}