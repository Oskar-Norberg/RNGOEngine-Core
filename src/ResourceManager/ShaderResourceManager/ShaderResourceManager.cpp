//
// Created by Oskar.Norberg on 2025-10-14.
//

#include "ResourceManager/ShaderResourceManager/ShaderResourceManager.h"

#include "Renderer/IRenderer.h"

RNGOEngine::Resources::ShaderResourceManager::ShaderResourceManager(
    RNGOEngine::Core::Renderer::IRenderer& renderer)
        : m_renderer(renderer)
{
}

RNGOEngine::Core::Renderer::ShaderID RNGOEngine::Resources::ShaderResourceManager::CreateShader(
    std::string_view source, Core::Renderer::ShaderType type)
{
    return m_renderer.CreateShader(source, type);
}

RNGOEngine::Core::Renderer::ShaderProgramID RNGOEngine::Resources::ShaderResourceManager::CreateShaderProgram(
    const Core::Renderer::ShaderID vertexShader, const Core::Renderer::ShaderID fragmentShader)
{
    return m_renderer.CreateShaderProgram(vertexShader, fragmentShader);
}

void RNGOEngine::Resources::ShaderResourceManager::DestroyShader(Core::Renderer::ShaderID shader)
{
    m_renderer.DestroyShader(shader);
}

void RNGOEngine::Resources::ShaderResourceManager::DestroyShaderProgram(
    Core::Renderer::ShaderProgramID program)
{
    m_renderer.DestroyShaderProgram(program);
}