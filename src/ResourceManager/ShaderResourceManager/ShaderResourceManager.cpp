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

RNGOEngine::Containers::Vectors::GenerationalKey<unsigned> RNGOEngine::Resources::ShaderResourceManager::
CreateShader(const std::string_view source, const Core::Renderer::ShaderType type)
{
    const auto shaderID = m_renderer.CreateShader(source, type);
    const auto shaderKey = m_shaders.Insert(shaderID);

    return shaderKey;
}

RNGOEngine::Containers::Vectors::GenerationalKey<unsigned> RNGOEngine::Resources::ShaderResourceManager::
CreateShaderProgram(Containers::Vectors::GenerationalKey<Core::Renderer::ShaderID> vertexShader,
                    Containers::Vectors::GenerationalKey<Core::Renderer::ShaderID> fragmentShader)
{
    const auto vertexShaderID = m_shaders.GetValidated(vertexShader);
    const auto fragmentShaderID = m_shaders.GetValidated(fragmentShader);

    if (!vertexShaderID || !fragmentShaderID)
    {
        RNGO_ASSERT(false && "ShaderResourceManager::CreateShaderProgram invalid key.");
        return m_shaderPrograms.InvalidKey();
    }

    const auto shaderProgramID = m_renderer.CreateShaderProgram(vertexShaderID.value(),
                                                                fragmentShaderID.value());
    const auto shaderProgramKey = m_shaderPrograms.Insert(shaderProgramID);

    return shaderProgramKey;
}

void RNGOEngine::Resources::ShaderResourceManager::MarkShaderForDestruction(
    Containers::Vectors::GenerationalKey<Core::Renderer::ShaderID> shader)
{
    if (!m_shaders.IsValid(shader))
    {
        RNGO_ASSERT(false && "ShaderResourceManager::DestroyShader invalid key.");
        return;
    }

    m_shaders.MarkForRemoval(shader);
}

void RNGOEngine::Resources::ShaderResourceManager::MarkShaderProgramForDestruction(
    Containers::Vectors::GenerationalKey<Core::Renderer::ShaderProgramID> program)
{
    if (!m_shaderPrograms.IsValid(program))
    {
        RNGO_ASSERT(false && "ShaderResourceManager::DestroyShaderProgram invalid key.");
        return;
    }

    m_shaderPrograms.MarkForRemoval(program);
}

std::optional<RNGOEngine::Core::Renderer::ShaderID> RNGOEngine::Resources::ShaderResourceManager::GetShader(
    const Containers::Vectors::GenerationalKey<Core::Renderer::ShaderID>& key) const
{
    return m_shaders.GetValidated(key);
}

std::optional<RNGOEngine::Core::Renderer::ShaderProgramID> RNGOEngine::Resources::ShaderResourceManager::
GetShaderProgram(const Containers::Vectors::GenerationalKey<Core::Renderer::ShaderProgramID>& key) const
{
    return m_shaderPrograms.GetValidated(key);
}

// TODO: actual destruction of marked shaders.