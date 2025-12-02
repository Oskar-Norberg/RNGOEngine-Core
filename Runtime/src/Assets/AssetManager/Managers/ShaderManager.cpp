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

    std::expected<ShaderProgramKey, ShaderManagerError> ShaderManager::UploadShader(
        std::string_view vertexShader, std::string_view fragmentShader
    )
    {
        const auto vertexShaderKey = CreateShader(vertexShader, Core::Renderer::ShaderType::Vertex);
        const auto fragmentShaderKey = CreateShader(fragmentShader, Core::Renderer::ShaderType::Fragment);

        const auto shaderProgramKey = CreateShaderProgram(vertexShaderKey, fragmentShaderKey);

        return shaderProgramKey;
    }

    void ShaderManager::DestroyShader(ShaderProgramKey shaderAsset)
    {
        // TODO:
    }

    Containers::GenerationalKey<Core::Renderer::ShaderID> ShaderManager::CreateShader(
        std::string_view shaderSource, Core::Renderer::ShaderType type
    )
    {
        auto& shaderResourceManager = m_resourceManager.GetShaderResourceManager();
        return shaderResourceManager.CreateShader(shaderSource, type);
    }

    Containers::GenerationalKey<Core::Renderer::ShaderProgramID> ShaderManager::CreateShaderProgram(
        Containers::GenerationalKey<Core::Renderer::ShaderID> vertexShaderKey,
        Containers::GenerationalKey<Core::Renderer::ShaderID> fragmentShaderKey
    )
    {
        auto& shaderResourceManager = m_resourceManager.GetShaderResourceManager();

        return shaderResourceManager.CreateShaderProgram(vertexShaderKey, fragmentShaderKey);
    }
}