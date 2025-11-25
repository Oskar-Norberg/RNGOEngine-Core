//
// Created by ringo on 2025-11-02.
//

#include "Assets/AssetImporters/ModelImporter.h"

#include "Assets/AssetDatabase/AssetDatabase.h"
#include "Assets/AssetLoaders/ModelLoaders/OBJModelLoader.h"
#include "Assets/AssetManager/AssetManager.h"
#include "Utilities/RNGOAsserts.h"

namespace RNGOEngine::AssetHandling
{
    ModelImporter::ModelImporter(const bool doFlipUVs)
        : m_doFlipUVs(doFlipUVs)
    {
    }

    std::expected<std::unique_ptr<Asset>, ImportingError> ModelImporter::Load(const AssetMetadata& metadata)
    {
        const auto& typedMetadata = static_cast<const ModelMetadata&>(metadata);

        const auto extension = typedMetadata.Path.extension().string();

        std::expected<ModelLoading::ModelData, ModelLoading::ModelLoadingError> modelHandle;

        // TODO: Temporarily disable OBJ loader until it's fully functional.
#if 0
        // OBJ Loader
        if (extension == ".obj")
        {
            modelHandle = ModelLoading::OBJModelLoader::LoadModel(typedMetadata.Path, m_doFlipUVs);
        }
        // Assimp Loader
        else if (extension == ".fbx" || extension == ".gltf")
        {
            modelHandle = ModelLoading::AssimpModelLoader::LoadModel(typedMetadata.Path, m_doFlipUVs);
        }
        else
        {
            RNGO_ASSERT(false && "ModelAssetImporter::Load - Unsupported model format");
            return;
        }
#endif
#if 1
        modelHandle = ModelLoading::AssimpModelLoader::LoadModel(typedMetadata.Path, m_doFlipUVs);
#endif
        
        if (!modelHandle)
        {
            // TODO: More specific error types
            return std::unexpected(ImportingError::UnknownError);
        }

        // Upload to GPU
        const auto result = AssetManager::GetInstance().GetModelManager().UploadModel(
            typedMetadata.UUID, modelHandle.value()
        );
        if (!result)
        {
            return std::unexpected(ImportingError::UnknownError);
        }

        return std::make_unique<ModelAsset>(result.value());
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