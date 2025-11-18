//
// Created by ringo on 2025-11-02.
//

#include "Assets/AssetDatabase/AssetDatabase.h"
#include "Assets/AssetImporters/AssimpModelImporter.h"
#include "Assets/AssetLoaders/ModelLoader.h"
#include "Assets/AssetManager/AssetManager.h"
#include "Utilities/RNGOAsserts.h"

namespace RNGOEngine::AssetHandling
{
    AssimpModelImporter::AssimpModelImporter(const bool doFlipUVs)
        : m_doFlipUVs(doFlipUVs)
    {
    }

    void AssimpModelImporter::Load(const AssetMetadata& metadata)
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

    void AssimpModelImporter::Unload(const AssetHandle& handle)
    {
        AssetManager::GetInstance().GetModelManager().UnloadModel(handle);
    }

    std::unique_ptr<AssetMetadata> AssimpModelImporter::CreateDefaultMetadata(const std::filesystem::path& path) const
    {
        auto modelData = std::make_unique<ModelMetadata>();
        modelData->Type = AssetType::Model;
        return std::move(modelData);
    }

    std::span<const std::string_view> AssimpModelImporter::GetSupportedExtensions() const
    {
        static constexpr std::string_view supportedTypes[] = {
            ".obj",
            ".fbx",
            ".gltf"
        };

        return supportedTypes;
    }
}