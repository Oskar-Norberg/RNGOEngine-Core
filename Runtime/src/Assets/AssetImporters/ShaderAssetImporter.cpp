//
// Created by ringo on 2025-11-02.
//

#include "Assets/AssetImporters/ShaderAssetImporter.h"

#include "Assets/AssetDatabase/AssetDatabase.h"
#include "Assets/AssetManager/AssetManager.h"
#include "Renderer/RenderID.h"

namespace RNGOEngine::AssetHandling
{
    void ShaderAssetImporter::Load(const AssetMetadata& metadata)
    {
        auto& typedMetadata = static_cast<const ShaderMetadata&>(metadata);

        // Preprocess Shader
        const auto shaderResult = m_shaderLoader.LoadShader(typedMetadata.Path);
        if (!shaderResult)
        {
            RNGO_ASSERT(false && "ShaderAssetImporter::Load failed to load shader.");
            // TODO: Return expected?
            return;
        }

        // TODO: Really, really, really, unstable way to determine shader type. Works for now!
        // const auto type = typedMetadata.Path.extension() == ".vert" ? Core::Renderer::ShaderType::Vertex
        //                                               : Core::Renderer::ShaderType::Fragment;

        // Upload Resources
        AssetManager::GetInstance().GetShaderManager().UploadShader(
            typedMetadata.UUID, shaderResult.value(), typedMetadata.ShaderType);
    }

    void ShaderAssetImporter::Unload(const AssetHandle& handle)
    {
        AssetManager::GetInstance().GetShaderManager().DestroyShader(handle);
    }

    std::unique_ptr<AssetMetadata> ShaderAssetImporter::CreateDefaultMetadata(const std::filesystem::path& path) const
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
            RNGO_ASSERT(false && "ShaderAssetImporter::CreateDefaultMetadata - Unsupported shader extension.");
        }
        
        return std::move(shaderMetadata);
    }

    std::span<const std::string_view> ShaderAssetImporter::GetSupportedExtensions() const
    {
        static constexpr std::string_view supportedTypes[] = {
            ".frag",
            ".vert"
        };

        return supportedTypes;
    }
}