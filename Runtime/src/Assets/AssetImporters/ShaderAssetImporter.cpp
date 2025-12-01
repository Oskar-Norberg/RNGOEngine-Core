//
// Created by ringo on 2025-11-02.
//

#include "Assets/AssetImporters/ShaderAssetImporter.h"

#include "Assets/AssetDatabase/AssetDatabase.h"
#include "Assets/AssetManager/AssetManager.h"
#include "Renderer/RenderID.h"

namespace RNGOEngine::AssetHandling
{
    ImportingError ShaderAssetImporter::LoadFromDisk(
        RuntimeAssetRegistry& registry, const AssetMetadata& metadata
    )
    {
        const auto* typedMetadata = dynamic_cast<const ShaderMetadata*>(&metadata);
        if (!typedMetadata)
        {
            RNGO_ASSERT(false && "ModelAssetImporter::Load - Metadata type mismatch.");
        }
        // TODO: This is shit, but it works.
        auto sharedCopy = std::make_shared<ShaderMetadata>(*typedMetadata);
        auto& safeTypedMetadata = *sharedCopy;

        // Preprocess Shader
        const auto shaderResult = m_shaderLoader.LoadShader(safeTypedMetadata.Path);
        if (!shaderResult)
        {
            switch (shaderResult.error())
            {
                case Shaders::ShaderPreProcessingError::FileNotFound:
                    return ImportingError::FileNotFound;
                case Shaders::ShaderPreProcessingError::MalformedInclude:
                case Shaders::ShaderPreProcessingError::TokenNotFound:
                    return ImportingError::MalformedFile;
                default:
                    return ImportingError::UnknownError;
            }
        }
        const auto& shaderString = shaderResult.value();

        // Upload Resources
        auto uploadResult = AssetManager::GetInstance().GetShaderManager().UploadShader(
            safeTypedMetadata.UUID, shaderString, safeTypedMetadata.ShaderType
        );

        if (!uploadResult)
        {
            switch (uploadResult.error())
            {
                // TODO: Because we're processing multiple shaders, how should we best return errors?
                case ShaderManagerError::None:
                    return ImportingError::UnknownError;
            }
        }

        auto& entry = registry.Insert<ShaderAsset>(safeTypedMetadata.UUID, std::move(uploadResult.value()));
        entry.SetState(AssetState::Ready);

        return ImportingError::None;
    }

    ImportingError ShaderAssetImporter::FinalizeLoad(
        Data::ThreadType threadType, RuntimeAssetRegistry& registry
    )
    {
        // TODO: For now, shaders are finalized in the load from disk.
        return ImportingError::None;

        // constexpr auto NUMBER_OF_SHADERS_TO_PROCESS_PER_CALL = 8;
        // for (int i = 0; i < NUMBER_OF_SHADERS_TO_PROCESS_PER_CALL && !m_shaderDataQueue.IsEmpty(); ++i)
        // {
        //     auto [shaderMetadata, shaderString] = m_shaderDataQueue.Dequeue();
        //
        //     // Upload Resources
        //     auto uploadResult = AssetManager::GetInstance().GetShaderManager().UploadShader(
        //         shaderMetadata.UUID, shaderString, shaderMetadata.ShaderType
        //     );
        //
        //     if (!uploadResult)
        //     {
        //         switch (uploadResult.error())
        //         {
        //             // TODO: Because we're processing multiple shaders, how should we best return errors?
        //             case ShaderManagerError::None:
        //                 return ImportingError::UnknownError;
        //         }
        //     }
        //
        //     auto& entry = registry.Insert<ShaderAsset>(shaderMetadata.UUID, std::move(uploadResult.value()));
        //     entry.SetState(AssetState::Ready);
        // }
    }

    void ShaderAssetImporter::Unload(const AssetHandle& handle)
    {
        AssetManager::GetInstance().GetShaderManager().DestroyShader(handle);
    }

    std::unique_ptr<AssetMetadata> ShaderAssetImporter::CreateDefaultMetadata(
        const std::filesystem::path& path
    ) const
    {
        auto shaderMetadata = std::make_unique<ShaderMetadata>();

        shaderMetadata->Type = AssetType::Shader;

        if (path.extension() == ".vert")
        {
            shaderMetadata->ShaderType = Core::Renderer::ShaderType::Vertex;
        }
        else if (path.extension() == ".frag")
        {
            shaderMetadata->ShaderType = Core::Renderer::ShaderType::Fragment;
        }
        else
        {
            RNGO_ASSERT(
                false && "ShaderAssetImporter::CreateDefaultMetadata - Unsupported shader extension."
            );
        }

        return std::move(shaderMetadata);
    }

    std::span<const std::string_view> ShaderAssetImporter::GetSupportedExtensions() const
    {
        static constexpr std::string_view supportedTypes[] = {".frag", ".vert"};

        return supportedTypes;
    }
}