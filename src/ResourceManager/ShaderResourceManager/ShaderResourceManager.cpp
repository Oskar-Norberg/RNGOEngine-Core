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
    const auto vertexShaderID = m_shaders.GetUnmarkedValidated(vertexShader);
    const auto fragmentShaderID = m_shaders.GetUnmarkedValidated(fragmentShader);

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
    if (!m_shaders.IsValidUnmarked(shader))
    {
        RNGO_ASSERT(false && "ShaderResourceManager::DestroyShader invalid key.");
        return;
    }

    m_shaders.MarkForRemoval(shader);
}

void RNGOEngine::Resources::ShaderResourceManager::MarkShaderProgramForDestruction(
    Containers::Vectors::GenerationalKey<Core::Renderer::ShaderProgramID> program)
{
    if (!m_shaderPrograms.IsValidUnmarked(program))
    {
        RNGO_ASSERT(false && "ShaderResourceManager::DestroyShaderProgram invalid key.");
        return;
    }

    m_shaderPrograms.MarkForRemoval(program);
}

std::optional<RNGOEngine::Core::Renderer::ShaderID> RNGOEngine::Resources::ShaderResourceManager::GetShader(
    const Containers::Vectors::GenerationalKey<Core::Renderer::ShaderID>& key) const
{
    return m_shaders.GetUnmarkedValidated(key);
}

std::optional<RNGOEngine::Core::Renderer::ShaderProgramID> RNGOEngine::Resources::ShaderResourceManager::
GetShaderProgram(const Containers::Vectors::GenerationalKey<Core::Renderer::ShaderProgramID>& key) const
{
    return m_shaderPrograms.GetUnmarkedValidated(key);
}

void RNGOEngine::Resources::ShaderResourceManager::MarkAllShaders()
{
    for (const auto key : m_shaders.Live())
    {
        m_shaders.MarkForRemoval(key);
    }
}

void RNGOEngine::Resources::ShaderResourceManager::DestroyMarkedShaders()
{
    for (const auto key : m_shaders.Marked())
    {
        m_renderer.DestroyShader(m_shaders.GetMarked(key));
        m_shaders.Remove(key);
    }
}

void RNGOEngine::Resources::ShaderResourceManager::MarkAllShaderPrograms()
{
    for (const auto key : m_shaderPrograms.Live())
    {
        m_shaderPrograms.MarkForRemoval(key);
    }
}

void RNGOEngine::Resources::ShaderResourceManager::DestroyMarkedShadersPrograms()
{
    for (const auto key : m_shaderPrograms.Marked())
    {
        m_renderer.DestroyShaderProgram(m_shaderPrograms.GetMarked(key));
        m_shaderPrograms.Remove(key);
    }
}
