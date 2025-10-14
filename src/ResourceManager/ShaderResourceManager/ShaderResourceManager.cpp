//
// Created by Oskar.Norberg on 2025-10-14.
//

#include "ResourceManager/ShaderResourceManager/ShaderResourceManager.h"

RNGOEngine::Resources::ShaderResourceManager::ShaderResourceManager(
    RNGOEngine::Core::Renderer::IRenderer& renderer)
        : m_renderer(m_renderer)
{
}

RNGOEngine::Core::Renderer::ShaderID RNGOEngine::Resources::ShaderResourceManager::CreateShader(
    std::string_view source, Core::Renderer::ShaderType type)
{
    return m_renderer.CreateShader(source, type);
}

RNGOEngine::Core::Renderer::ShaderProgramID RNGOEngine::Resources::ShaderResourceManager::CreateShaderProgram(
    Core::Renderer::ShaderID vertexShader, Core::Renderer::ShaderID fragmentShader)
{
}

void RNGOEngine::Resources::ShaderResourceManager::DestroyShader(Core::Renderer::ShaderID shader)
{
}

void RNGOEngine::Resources::ShaderResourceManager::DestroyShaderProgram(
    Core::Renderer::ShaderProgramID program)
{
}