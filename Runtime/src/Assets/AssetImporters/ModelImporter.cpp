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

    ImportingError ModelImporter::LoadFromDisk(RuntimeAssetRegistry& registry, const AssetMetadata& metadata)
    {
        const auto* typedMetadata = dynamic_cast<const ModelMetadata*>(&metadata);
        if (!typedMetadata)
        {
            RNGO_ASSERT(false && "ModelAssetImporter::Load - Metadata type mismatch.");
        }
        // TODO: This is shit, but it works.
        auto sharedCopy = std::make_shared<ModelMetadata>(*typedMetadata);
        auto& safeTypedMetadata = *sharedCopy;

        auto extension = safeTypedMetadata.Path.extension().string();
        std::transform(extension.begin(), extension.end(), extension.begin(), tolower);

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
        modelHandle = ModelLoading::AssimpModelLoader::LoadModel(safeTypedMetadata.Path, m_doFlipUVs);
#endif

        if (!modelHandle)
        {
            // TODO: More specific error types
            return ImportingError::UnknownError;
        }

        m_modelDataQueue.Enqueue(std::make_pair(safeTypedMetadata, std::move(modelHandle.value())));

        return ImportingError::None;
    }

    ImportingError ModelImporter::FinalizeLoad(Data::ThreadType threadType, RuntimeAssetRegistry& registry)
    {
        constexpr auto NUMBER_OF_MODELS_TO_PROCESS_PER_CALL = 8;
        // TODO: Is having a inline boolean in the for-loop cursed?
        for (int i = 0; i < NUMBER_OF_MODELS_TO_PROCESS_PER_CALL && !m_modelDataQueue.IsEmpty(); ++i)
        {
            auto [modelMetadata, modelHandle] = m_modelDataQueue.Dequeue();

            // Upload to GPU
            auto result =
                AssetManager::GetInstance().GetModelManager().UploadModel(ModelHandle{modelMetadata.UUID}, modelHandle);
            auto& asset = result.value();

            if (!result)
            {
                return ImportingError::UnknownError;
            }

            auto& entry = registry.Insert<ModelAsset>(ModelHandle{modelMetadata.UUID}, std::move(asset));
            entry.SetState(AssetState::Ready);
        }

        return ImportingError::None;
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