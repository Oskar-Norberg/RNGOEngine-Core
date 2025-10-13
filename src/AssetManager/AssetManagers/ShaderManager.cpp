//
// Created by Oskar.Norberg on 2025-09-11.
//

#include "AssetManager/AssetManagers/ShaderManager.h"

#include "ResourceManager/ResourceManager.h"

namespace RNGOEngine::AssetHandling
{
    ShaderManager::ShaderManager(Resources::ResourceManager& resourceManager,
                                 const AssetFileFetcher& assetFetcher)
        : m_shaderLoader(assetFetcher), m_resourceManager(resourceManager)

    {
    }

    std::expected<Core::Renderer::ShaderProgramID, ShaderManagerError> ShaderManager::LoadShaderProgram(
        const std::filesystem::path& vertexShaderPath, const std::filesystem::path& fragmentShaderPath)
    {
        const auto vertexShader = CreateShader(vertexShaderPath, Core::Renderer::ShaderType::Vertex);
        if (!vertexShader.has_value())
        {
            return std::unexpected(vertexShader.error());
        }

        const auto fragmentShader = CreateShader(fragmentShaderPath, Core::Renderer::ShaderType::Fragment);
        if (!fragmentShader.has_value())
        {
            return std::unexpected(fragmentShader.error());
        }

        const auto shaderProgram = CreateShaderProgram(vertexShader.value(), fragmentShader.value());
        if (!shaderProgram.has_value())
        {
            return std::unexpected(shaderProgram.error());
        }

        return shaderProgram.value();
    }

    std::expected<Core::Renderer::ShaderID, ShaderManagerError> ShaderManager::CreateShader(
        const std::filesystem::path& path, Core::Renderer::ShaderType type)
    {
        // Is already cached?
        if (const auto shader = m_shaderCache.TryGet(path); shader.has_value())
        {
            return shader.value();
        }

        const auto shaderSource = m_shaderLoader.LoadShader(path);
        if (!shaderSource.has_value())
        {
            switch (shaderSource.error())
            {
                // TODO: Get feedback on this pattern, I think this looks really ugly.
                case Shaders::ShaderPreProcessingError::None:
                    return std::unexpected(ShaderManagerError::None);
                case Shaders::ShaderPreProcessingError::FileNotFound:
                    return std::unexpected(ShaderManagerError::FileNotFound);
                case Shaders::ShaderPreProcessingError::MalformedInclude:
                    return std::unexpected(ShaderManagerError::MalformedInclude);
                case Shaders::ShaderPreProcessingError::TokenNotFound:
                    return std::unexpected(ShaderManagerError::TokenNotFound);
            }
        }

        const auto shaderID = m_resourceManager.CreateShader(shaderSource.value(), type);
        m_shaderCache.Insert(path, shaderID);

        return shaderID;
    }

    std::expected<Core::Renderer::ShaderProgramID, ShaderManagerError> ShaderManager::CreateShaderProgram(
        const Core::Renderer::ShaderID vertexShader, const Core::Renderer::ShaderID fragmentShader)
    {
        // Is already cached?
        const auto shaderPair = std::make_pair(vertexShader, fragmentShader);
        if (const auto shaderProgram = m_shaderProgramCache.TryGet(shaderPair); shaderProgram.has_value())
        {
            return shaderProgram.value();
        }

        const auto shaderProgram = m_resourceManager.CreateShaderProgram(vertexShader, fragmentShader);
        m_shaderProgramCache.Insert(shaderPair, shaderProgram);
        
        return shaderProgram;
    }
}