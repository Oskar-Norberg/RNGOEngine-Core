//
// Created by ringo on 2025-11-02.
//

#include "Assets/AssetImporters/ModelImporter.h"

#include "Assets/AssetDatabase/AssetDatabase.h"
#include "Assets/AssetManager/AssetManager.h"
#include "Utilities/RNGOAsserts.h"

namespace RNGOEngine::AssetHandling
{
    ModelImporter::ModelImporter(const bool doFlipUVs)
        : m_doFlipUVs(doFlipUVs)
    {
    }

    void ModelImporter::Load(const AssetMetadata& metadata)
    {
        const auto& typedMetadata = static_cast<const ModelMetadata&>(metadata);

        const auto extension = typedMetadata.Path.extension().string();

        // Load Model into RAM
        const auto modelHandle =
            ModelLoading::AssimpModelLoader::LoadModel(typedMetadata.Path, m_doFlipUVs);

        if (!modelHandle)
        {
            // TODO: Return expected instead of directly returning handle?
            RNGO_ASSERT(false && "ModelAssetImporter::Load - Failed to Load Model");
        }

        // Upload to GPU
        const auto errorMessage = AssetManager::GetInstance().GetModelManager().UploadModel(
            typedMetadata.UUID, modelHandle.value()
        );
        if (errorMessage != ModelCreationError::None)
        {
            // TODO: Error handling
            RNGO_ASSERT(false && "ModelAssetImporter::Load - Failed to Load Model");
        }

        // OBJ Loader
        if (extension == ".obj")
        {
        }
        // Assimp Loader
        else if (extension == ".fbx" || extension == ".gltf")
        {
           
        }
    }

    void ModelImporter::Unload(const AssetHandle& handle)
    {
        AssetManager::GetInstance().GetModelManager().UnloadModel(handle);
    }

    std::unique_ptr<AssetMetadata> ModelImporter::CreateDefaultMetadata(
        const std::filesystem::path& path
    ) const
    {
        auto modelData = std::make_unique<ModelMetadata>();
        modelData->Type = AssetType::Model;
        return std::move(modelData);
    }

    std::span<const std::string_view> ModelImporter::GetSupportedExtensions() const
    {
        static constexpr std::string_view supportedTypes[] = {".obj", ".fbx", ".gltf"};

        return supportedTypes;
    }
}