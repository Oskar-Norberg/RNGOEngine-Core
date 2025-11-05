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
    ModelAssetImporter::ModelAssetImporter(const bool doFlipUVs)
        : m_doFlipUVs(doFlipUVs)
    {
    }

    void ModelAssetImporter::Load(const AssetMetadata& metadata)
    {
        const auto& typedMetadata = static_cast<const ModelMetadata&>(metadata);

        // Load Model into RAM
        const auto modelHandle = ModelLoading::LoadModel(typedMetadata.Path, m_doFlipUVs);
        if (!modelHandle)
        {
            // TODO: Return expected instead of directly returning handle?
            RNGO_ASSERT(false && "ModelAssetImporter::Load - Failed to Load Model");
        }

        // Upload to GPU
        const auto errorMessage =
            AssetManager::GetInstance().GetModelManager().
                                        UploadModel(typedMetadata.UUID, modelHandle.value());
        if (errorMessage != ModelCreationError::None)
        {
            // TODO: Error handling
            RNGO_ASSERT(false && "ModelAssetImporter::Load - Failed to Load Model");
        }

        // Unload Model from RAM
        ModelLoading::UnloadModel(modelHandle.value());
    }

    void ModelAssetImporter::Unload(const AssetHandle& handle)
    {
        AssetManager::GetInstance().GetModelManager().UnloadModel(handle);
    }

    std::unique_ptr<AssetMetadata> ModelAssetImporter::CreateDefaultMetadata() const
    {
        return std::make_unique<ModelMetadata>();
    }

    std::span<const std::string_view> ModelAssetImporter::GetSupportedExtensions() const
    {
        static constexpr std::string_view supportedTypes[] = {
            ".obj",
            ".fbx",
            ".gltf"
        };

        return supportedTypes;
    }
}