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
    ModelAssetImporter::ModelAssetImporter(AssetDatabase& assetDatabase, AssetManager& assetManager,
                                           const bool doFlipUVs)
        : AssetImporter(assetDatabase, assetManager), m_doFlipUVs(doFlipUVs)
    {
    }

    AssetHandle ModelAssetImporter::Load(const std::filesystem::path& path)
    {
        // Already in database?
        if (m_assetDatabase.IsRegistered(path))
        {
            // TODO: Reload if invalid?
            return m_assetDatabase.GetAssetHandle(path);
        }

        // Register in Database
        const auto& assetHandle = m_assetDatabase.RegisterAsset<ModelMetadata>(path);

        // Load Model into RAM
        const auto modelHandle = ModelLoading::LoadModel(path, m_doFlipUVs);
        if (!modelHandle)
        {
            // TODO: Return expected instead of directly returning handle?
            RNGO_ASSERT(false && "ModelAssetImporter::Load - Failed to Load Model");
        }

        // Upload to GPU
        const auto errorMessage = m_assetManager.GetModelManager().UploadModel(assetHandle, modelHandle.value());
        if (errorMessage != ModelCreationError::None)
        {
            // TODO: Error handling
            RNGO_ASSERT(false && "ModelAssetImporter::Load - Failed to Load Model");
        }

        // Unload Model from RAM
        ModelLoading::UnloadModel(modelHandle.value());

        // Mark Valid
        auto& metadata = m_assetDatabase.GetAssetMetadataAs<ModelMetadata>(assetHandle);
        metadata.State = AssetState::Valid;

        return assetHandle;
    }
}