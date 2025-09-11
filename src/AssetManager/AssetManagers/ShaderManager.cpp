//
// Created by Oskar.Norberg on 2025-09-11.
//

#include "AssetManager/AssetManagers/ShaderManager.h"

namespace RNGOEngine::AssetHandling
{
    ShaderManager::ShaderManager(Core::Renderer::IRenderer& renderer)
        : m_renderer(renderer)
    {
    }

    std::expected<Core::Renderer::ShaderID, ShaderManagerError> ShaderManager::CreateShader(
        const std::string_view shaderSource, const Core::Renderer::ShaderType type)
    {
        // TODO: The renderer needs to return errors.
        return m_renderer.CreateShader(shaderSource, type);
    }

    std::expected<Core::Renderer::ShaderProgramID, ShaderManagerError> ShaderManager::CreateShaderProgram(
        const Core::Renderer::ShaderID vertexShader, const Core::Renderer::ShaderID fragmentShader)
    {
        return m_renderer.CreateShaderProgram(vertexShader, fragmentShader);
    }
}