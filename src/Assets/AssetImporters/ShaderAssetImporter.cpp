//
// Created by ringo on 2025-11-02.
//

#include "Assets/AssetImporters/ShaderAssetImporter.h"

#include "Assets/AssetDatabase/AssetDatabase.h"
#include "Assets/AssetManager/AssetManager.h"
#include "Renderer/RenderID.h"

namespace RNGOEngine::AssetHandling
{
    AssetHandle ShaderAssetImporter::Register(const std::filesystem::path& path)
    {
        auto& databaseRef = AssetDatabase::GetInstance();
        
        if (databaseRef.IsRegistered(path))
        {
            // TODO: Check for valid type?
            return databaseRef.GetAssetHandle(path);
        }

        // Register in Database
        const auto& assetHandle = databaseRef.RegisterAsset<ShaderMetadata>(path);
        auto& metadata = databaseRef.GetAssetMetadataAs<ShaderMetadata>(assetHandle);

        metadata.Path = path;
        metadata.State = AssetState::Invalid;
        metadata.Type = AssetType::Shader;

        return assetHandle;
    }

    void ShaderAssetImporter::Unregister(const AssetHandle& handle)
    {
        auto& databaseRef = AssetDatabase::GetInstance();
        
        if (!databaseRef.IsRegistered(handle))
        {
            return;
        }

        auto& assetMetadata = databaseRef.GetAssetMetadataAs<ShaderMetadata>(handle);
        if (assetMetadata.State == AssetState::Valid)
        {
            Unload(handle);
        }

        databaseRef.UnregisterAsset<ShaderMetadata>(handle);
    }

    AssetHandle ShaderAssetImporter::Load(const std::filesystem::path& path)
    {
        auto& databaseRef = AssetDatabase::GetInstance();

        auto assetHandle = Register(path);
        auto& metadata = databaseRef.GetAssetMetadataAs<ShaderMetadata>(assetHandle);

        // Already Loaded?
        if (metadata.State == AssetState::Valid)
        {
            return assetHandle;
        }

        // Preprocess Shader
        const auto shaderResult = m_shaderLoader.LoadShader(path);
        if (!shaderResult)
        {
            RNGO_ASSERT(false && "ShaderAssetImporter::Load failed to load shader.");
            // TODO: Return error handle or expected.
            return {};
        }

        // TODO: Really, really, really, unstable way to determine shader type. Works for now!
        const auto type = path.extension() == ".vert" ? Core::Renderer::ShaderType::Vertex
                                                      : Core::Renderer::ShaderType::Fragment;

        // Upload Resources
        AssetManager::GetInstance().GetShaderManager().UploadShader(assetHandle, shaderResult.value(), type);

        // Mark Valid
        metadata.State = AssetState::Valid;
        metadata.ShaderType = type;

        return assetHandle;
    }

    void ShaderAssetImporter::Unload(const AssetHandle& handle)
    {
        auto& databaseRef = AssetDatabase::GetInstance();

        if (!databaseRef.IsRegistered(handle))
        {
            return;
        }

        auto& assetMetadata = databaseRef.GetAssetMetadataAs<ShaderMetadata>(handle);
        if (assetMetadata.State != AssetState::Valid)
        {
            // Shader already unloaded.
            return;
        }
        
        AssetManager::GetInstance().GetShaderManager().DestroyShader(handle);
        assetMetadata.State = AssetState::Invalid;
    }
}