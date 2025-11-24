//
// Created by Oskar.Norberg on 2025-09-11.
//

#include "Assets/AssetManager/Managers/ShaderManager.h"

#include "ResourceManager/ResourceManager.h"

namespace RNGOEngine::AssetHandling
{
    ShaderManager::ShaderManager(Resources::ResourceManager& resourceManager)
        : m_resourceManager(resourceManager)
    {
    }

    std::expected<std::weak_ptr<Asset>, ShaderManagerError> ShaderManager::UploadShader(
        const AssetHandle& assetHandle, const std::string_view shaderSource,
        const Core::Renderer::ShaderType type
    )
    {
        auto& shaderResourceManager = m_resourceManager.GetShaderResourceManager();

        // Create Shader Resource
        auto shaderAsset = std::make_shared<ShaderAsset>(
            AssetHandle(assetHandle), shaderResourceManager.CreateShader(shaderSource, type), type
        );
        // Store Runtime shader data
        auto [it, inserted] = m_handleToShader.insert({assetHandle, std::move(shaderAsset)});

        return it->second;
    }

    void ShaderManager::DestroyShader(const AssetHandle& assetHandle)
    {
        // TODO:
    }

    // TODO: Long function, clean up.
    Containers::GenerationalKey<RuntimeShaderProgramData> ShaderManager::CreateShaderProgram(
        const AssetHandle& vertexShader, const AssetHandle& fragmentShader
    )
    {
        auto& shaderResourceManager = m_resourceManager.GetShaderResourceManager();

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

        const auto& vertexKey = m_handleToShader.at(vertexShader);
        const auto& fragmentKey = m_handleToShader.at(fragmentShader);

        // Type mismatch
        if (vertexKey->GetShaderType() != Core::Renderer::ShaderType::Vertex ||
            fragmentKey->GetShaderType() != Core::Renderer::ShaderType::Fragment)
        {
            RNGO_ASSERT(false && "ShaderManager::CreateShaderProgram shader type mismatch.");
            return {};
        }

        // Create ShaderProgram
        const auto shaderProgramKey =
            shaderResourceManager.CreateShaderProgram(vertexKey->GetShaderKey(), fragmentKey->GetShaderKey());

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
        auto& shaderResourceManager = m_resourceManager.GetShaderResourceManager();

        if (!validated)
        {
            // TODO: Return default? Return optional?
            return {};
        }

        const auto programKey = validated->get().ProgramKey;
        const auto shaderProgramOpt = shaderResourceManager.GetShaderProgram(programKey);
        if (!shaderProgramOpt)
        {
            // TODO Return default? Return optional?
            return {};
        }

        return shaderProgramOpt.value();
    }
}