//
// Created by Oskar.Norberg on 2025-09-11.
//

#include "Assets/AssetManager/Managers/ShaderManager.h"

#include "Assets/AssetTypes/ShaderAsset.h"
#include "ResourceManager/ResourceManager.h"

namespace RNGOEngine::AssetHandling
{
    ShaderManager::ShaderManager(AssetDatabase& assetDatabase, Resources::ResourceManager& resourceManager,
                                 const AssetFetcher& assetFetcher)
        : m_assetDatabase(assetDatabase),
          m_resourceManager(resourceManager),
          m_shaderLoader(assetFetcher)
    {
    }

    AssetHandle ShaderManager::CreateShader(const std::filesystem::path& path,
                                            Core::Renderer::ShaderType type)
    {
        // Exists in database
        if (m_assetDatabase.IsRegistered(path))
        {
            return m_assetDatabase.GetAssetHandle(path);
        }

        // Load Shader
        const auto shaderResult = m_shaderLoader.LoadShader(path);
        if (!shaderResult)
        {
            RNGO_ASSERT(false && "ShaderManager::CreateShader failed to load shader.");
            // TODO: Return error handle or expected.
            return {};
        }

        // Create Shader Resource
        const auto shaderID = m_resourceManager.CreateShader(shaderResult.value(), type);
        const auto runtimeShaderData = RuntimeShaderData{
            .Type = type,
            .ShaderKey = shaderID
        };
        // Store Runtime shader data
        auto runtimeShaderKey = m_shaders.Insert(runtimeShaderData);

        // Insert into Asset Database
        auto assetHandle = m_assetDatabase.RegisterAsset<ShaderMetadata>( path);
        auto& shaderMetadata = m_assetDatabase.GetAssetMetadata(assetHandle);
        // Mark Asset as valid
        shaderMetadata.State = AssetState::Valid;

        m_handleToShader.insert({assetHandle, runtimeShaderKey});

        return assetHandle;
    }

    // TODO: Long function, clean up.
    Containers::GenerationalKey<RuntimeShaderProgramData> ShaderManager::CreateShaderProgram(
        const AssetHandle& vertexShader, const AssetHandle& fragmentShader)
    {
        // Lots of double work on the vert/frag shaders here, make a helper function / lambda.
        // Invalid Shader AssetHandles
        if (!m_handleToShader.contains(vertexShader) || !m_handleToShader.contains(fragmentShader))
        {
            RNGO_ASSERT(false && "ShaderManager::CreateShaderProgram invalid shader handle.");
            return {};
        }

        const auto vertexKey = m_handleToShader.at(vertexShader);
        const auto fragmentKey = m_handleToShader.at(fragmentShader);

        const auto vertexRuntimeShader = m_shaders.GetUnmarkedValidated(vertexKey);
        const auto fragmentRuntimeShader = m_shaders.GetUnmarkedValidated(fragmentKey);

        // Invalid RuntimeShaderData/Runtime Data
        if (!vertexRuntimeShader || !fragmentRuntimeShader)
        {
            RNGO_ASSERT(false && "ShaderManager::CreateShaderProgram invalid runtime shader.");
            return {};
        }

        // Type mismatch
        if (vertexRuntimeShader->get().Type != Core::Renderer::ShaderType::Vertex ||
            fragmentRuntimeShader->get().Type != Core::Renderer::ShaderType::Fragment)
        {
            RNGO_ASSERT(false && "ShaderManager::CreateShaderProgram shader type mismatch.");
            return {};
        }

        // Create ShaderProgram
        const auto shaderProgramKey = m_resourceManager.CreateShaderProgram(
            vertexRuntimeShader->get().ShaderKey,
            fragmentRuntimeShader->get().ShaderKey);

        const auto runtimeShaderProgramData = RuntimeShaderProgramData{
            .ProgramKey = shaderProgramKey
        };

        // Add to runtime storage.
        return m_shaderPrograms.Insert(runtimeShaderProgramData);
    }

    Core::Renderer::ShaderProgramID ShaderManager::GetShaderProgram(
        const Containers::GenerationalKey<RuntimeShaderProgramData>& key)
    {
        const auto validated = m_shaderPrograms.GetUnmarkedValidated(key);

        if (!validated)
        {
            // TODO: Return default? Return optional?
            return {};
        }

        const auto programKey = validated->get().ProgramKey;
        const auto shaderProgramOpt = m_resourceManager.GetShaderProgram(programKey);
        if (!shaderProgramOpt)
        {
            // TODO Return default? Return optional?
            return {};
        }

        return shaderProgramOpt.value();
    }

    void ShaderManager::BeginDestroyAllShaders()
    {
        for (const auto shader : m_shaders.Live())
        {
            const auto validated = m_shaders.GetUnmarkedValidated(shader);
            if (validated)
            {
                m_resourceManager.MarkShaderForDestruction(validated->get().ShaderKey);
            }
        }
    }

    void ShaderManager::BeginDestroyAllShaderPrograms()
    {
        for (const auto program : m_shaderPrograms.Live())
        {
            const auto validated = m_shaderPrograms.GetUnmarkedValidated(program);
            if (validated)
            {
                m_resourceManager.MarkShaderProgramForDestruction(validated->get().ProgramKey);
            }
        }
    }
}