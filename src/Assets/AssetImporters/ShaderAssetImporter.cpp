//
// Created by ringo on 2025-11-02.
//

#include "Assets/AssetImporters/ShaderAssetImporter.h"

#include "Assets/AssetDatabase/AssetDatabase.h"
#include "Assets/AssetManager/AssetManager.h"
#include "Renderer/RenderID.h"

namespace RNGOEngine::AssetHandling
{
    ShaderAssetImporter::ShaderAssetImporter(
        AssetFetcher& assetFetcher, AssetDatabase& assetDatabase, AssetManager& assetManager
    )
        : AssetImporter(assetFetcher, assetDatabase, assetManager), m_shaderLoader(assetFetcher)
    {
    }

    AssetHandle ShaderAssetImporter::Register(const std::filesystem::path& path)
    {
        if (m_assetDatabase.IsRegistered(path))
        {
            // TODO: Check for valid type?
            return m_assetDatabase.GetAssetHandle(path);
        }

        // Register in Database
        const auto& assetHandle = m_assetDatabase.RegisterAsset<ShaderMetadata>(path);
        auto& metadata = m_assetDatabase.GetAssetMetadataAs<ShaderMetadata>(assetHandle);

        metadata.Path = path;
        metadata.State = AssetState::Invalid;
        metadata.Type = AssetType::Shader;

        return assetHandle;
    }

    void ShaderAssetImporter::Unregister(const AssetHandle& handle)
    {
        if (!m_assetDatabase.IsRegistered(handle))
        {
            return;
        }

        auto& assetMetadata = m_assetDatabase.GetAssetMetadataAs<ShaderMetadata>(handle);
        if (assetMetadata.State == AssetState::Valid)
        {
            Unload(handle);
        }

        m_assetDatabase.UnregisterAsset<ShaderMetadata>(handle);
    }

    AssetHandle ShaderAssetImporter::Load(const std::filesystem::path& path)
    {
        auto assetHandle = Register(path);
        auto& metadata = m_assetDatabase.GetAssetMetadataAs<ShaderMetadata>(assetHandle);

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
        m_assetManager.GetShaderManager().UploadShader(assetHandle, shaderResult.value(), type);

        // Mark Valid
        metadata.State = AssetState::Valid;
        metadata.ShaderType = type;

        return assetHandle;
    }

    void ShaderAssetImporter::Unload(const AssetHandle& handle)
    {
        if (!m_assetDatabase.IsRegistered(handle))
        {
            return;
        }

        auto& assetMetadata = m_assetDatabase.GetAssetMetadataAs<ShaderMetadata>(handle);
        if (assetMetadata.State != AssetState::Valid)
        {
            // Shader already unloaded.
            return;
        }
        
        m_assetManager.GetShaderManager().DestroyShader(handle);
        assetMetadata.State = AssetState::Invalid;
    }
}