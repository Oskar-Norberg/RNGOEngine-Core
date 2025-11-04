//
// Created by Oskar.Norberg on 2025-09-11.
//

#include "Assets/AssetManager/Managers/ShaderManager.h"

#include "Assets/AssetTypes/ShaderAsset.h"
#include "ResourceManager/ResourceManager.h"

namespace RNGOEngine::AssetHandling
{
    ShaderManager::ShaderManager(Resources::ResourceManager& resourceManager)
        : m_resourceManager(resourceManager)
    {
    }

    ShaderManagerError ShaderManager::UploadShader(
        const AssetHandle& assetHandle, const std::string_view shaderSource,
        const Core::Renderer::ShaderType type
    )
    {
        // Create Shader Resource
        const auto shaderID = m_resourceManager.CreateShader(shaderSource, type);
        const auto runtimeShaderData = RuntimeShaderData{.Type = type, .ShaderKey = shaderID};
        // Store Runtime shader data
        auto runtimeShaderKey = m_shaders.Insert(runtimeShaderData);

        m_handleToShader.insert({assetHandle, runtimeShaderKey});

        // TODO:
        return ShaderManagerError::None;
    }

    void ShaderManager::DestroyShader(const AssetHandle& assetHandle)
    {
        if (m_handleToShader.contains(assetHandle))
        {
            const auto& runtimeShaderKey = m_handleToShader.at(assetHandle);
            const auto runtimeShaderDataOpt = m_shaders.GetUnmarkedValidated(runtimeShaderKey);

            if (runtimeShaderDataOpt)
            {
                m_resourceManager.MarkShaderForDestruction(runtimeShaderDataOpt->get().ShaderKey);
            }

            m_shaders.Remove(runtimeShaderKey);
            m_handleToShader.erase(assetHandle);
        }
    }

    // TODO: Long function, clean up.
    Containers::GenerationalKey<RuntimeShaderProgramData> ShaderManager::CreateShaderProgram(
        const AssetHandle& vertexShader, const AssetHandle& fragmentShader
    )
    {
        // Check cache first
        const auto pair = std::make_pair(vertexShader, fragmentShader);
        if (m_shaderProgramCache.contains(pair))
        {
            const auto& cachedKey = m_shaderProgramCache.at(pair);
            // Validate cached key
            if (m_shaderPrograms.IsValidUnmarked(cachedKey))
            {
                return cachedKey;
            }
            else
            {
                m_shaderProgramCache.erase(pair);
            }
        }

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
            vertexRuntimeShader->get().ShaderKey, fragmentRuntimeShader->get().ShaderKey
        );

        const auto runtimeShaderProgramData = RuntimeShaderProgramData{.ProgramKey = shaderProgramKey};

        // Add to runtime storage.
        const auto key = m_shaderPrograms.Insert(runtimeShaderProgramData);

        // Add to program cache
        m_shaderProgramCache.insert({pair, key});

        return key;
    }

    Core::Renderer::ShaderProgramID ShaderManager::GetShaderProgram(
        const Containers::GenerationalKey<RuntimeShaderProgramData>& key
    )
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
}