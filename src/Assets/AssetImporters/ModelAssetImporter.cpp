//
// Created by ringo on 2025-11-02.
//

#include "Assets/AssetImporters/ModelAssetImporter.h"

#include "Assets/AssetDatabase/AssetDatabase.h"
#include "Assets/AssetLoaders/ModelLoader.h"
#include "Assets/AssetManager/AssetManager.h"
#include "Utilities/RNGOAsserts.h"

namespace RNGOEngine::AssetHandling
{
    ModelAssetImporter::ModelAssetImporter(
        AssetFetcher& assetFetcher, AssetDatabase& assetDatabase, AssetManager& assetManager,
        const bool doFlipUVs
    )
        : AssetImporter(assetFetcher, assetDatabase, assetManager), m_doFlipUVs(doFlipUVs)
    {
    }

    AssetHandle ModelAssetImporter::Register(const std::filesystem::path& path)
    {
        // Already in database?
        if (m_assetDatabase.IsRegistered(path))
        {
            // TODO: Check for valid type?
            return m_assetDatabase.GetAssetHandle(path);
        }

        // Register in Database
        // TODO: What happens if a path is registered with a different type?
        const auto& assetHandle = m_assetDatabase.RegisterAsset<ModelMetadata>(path);
        auto& metadata = m_assetDatabase.GetAssetMetadataAs<ModelMetadata>(assetHandle);
        metadata.State = AssetState::Invalid;
        metadata.Path = path;
        metadata.Type = AssetType::Model;

        return assetHandle;
    }

    void ModelAssetImporter::Unregister(const AssetHandle& handle)
    {
        if (m_assetDatabase.IsRegistered(handle))
        {
            if (const auto& metadata = m_assetDatabase.GetAssetMetadataAs<ModelMetadata>(handle);
                metadata.State == AssetState::Valid)
            {
                Unload(handle);
            }

            m_assetDatabase.UnregisterAsset<ModelMetadata>(handle);
        }
    }

    AssetHandle ModelAssetImporter::Load(const std::filesystem::path& path)
    {
        const auto& assetHandle = Register(path);
        auto& metadata = m_assetDatabase.GetAssetMetadataAs<ModelMetadata>(assetHandle);

        // Already Loaded.
        if (metadata.State == AssetState::Valid)
        {
            return assetHandle;
        }

        // Load Model into RAM
        const auto modelHandle = ModelLoading::LoadModel(path, m_doFlipUVs);
        if (!modelHandle)
        {
            // TODO: Return expected instead of directly returning handle?
            RNGO_ASSERT(false && "ModelAssetImporter::Load - Failed to Load Model");
        }

        // Upload to GPU
        const auto errorMessage =
            m_assetManager.GetModelManager().UploadModel(assetHandle, modelHandle.value());
        if (errorMessage != ModelCreationError::None)
        {
            // TODO: Error handling
            RNGO_ASSERT(false && "ModelAssetImporter::Load - Failed to Load Model");
        }

        // Unload Model from RAM
        ModelLoading::UnloadModel(modelHandle.value());

        // Mark Valid
        metadata.State = AssetState::Valid;

        return assetHandle;
    }

    void ModelAssetImporter::Unload(const AssetHandle& handle)
    {
        if (!m_assetDatabase.IsRegistered(handle))
        {
            return;
        }

        auto& assetMetadata = m_assetDatabase.GetAssetMetadataAs<ModelMetadata>(handle);
        m_assetManager.GetModelManager().UnloadModel(handle);
        assetMetadata.State = AssetState::Invalid;
    }
}