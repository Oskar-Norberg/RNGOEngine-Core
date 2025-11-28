//
// Created by ringo on 2025-11-02.
//

#include "Assets/AssetImporters/ShaderAssetImporter.h"

#include "Assets/AssetDatabase/AssetDatabase.h"
#include "Assets/AssetManager/AssetManager.h"
#include "Renderer/RenderID.h"

namespace RNGOEngine::AssetHandling
{
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

    std::expected<ShaderAsset, ImportingError> ShaderAssetImporter::ImportAsset(const AssetMetadata& metadata)
    {
        auto& typedMetadata = static_cast<const ShaderMetadata&>(metadata);

        // Preprocess Shader
        const auto shaderResult = m_shaderLoader.LoadShader(typedMetadata.Path);
        if (!shaderResult)
        {
            switch (shaderResult.error())
            {
                case Shaders::ShaderPreProcessingError::FileNotFound:
                    return std::unexpected(ImportingError::FileNotFound);
                case Shaders::ShaderPreProcessingError::MalformedInclude:
                    return std::unexpected(ImportingError::MalformedFile);
                case Shaders::ShaderPreProcessingError::TokenNotFound:
                    return std::unexpected(ImportingError::MalformedFile);
                default:
                    return std::unexpected(ImportingError::UnknownError);
            }
        }

        // TODO: Really, really, really, unstable way to determine shader type. Works for now!
        // const auto type = typedMetadata.Path.extension() == ".vert" ? Core::Renderer::ShaderType::Vertex
        //                                               : Core::Renderer::ShaderType::Fragment;

        // Upload Resources
        auto uploadResult = AssetManager::GetInstance().GetShaderManager().UploadShader(
            typedMetadata.UUID, shaderResult.value(), typedMetadata.ShaderType
        );

        if (!uploadResult)
        {
            switch (uploadResult.error())
            {
                case ShaderManagerError::None:
                    return std::unexpected(ImportingError::UnknownError);
            }
        }

        return uploadResult.value();
    }

    AssetType ShaderAssetImporter::GetAssetType() const
    {
        return AssetType::Shader;
    }
}