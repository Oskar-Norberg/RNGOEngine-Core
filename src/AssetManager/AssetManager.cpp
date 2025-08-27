//
// Created by Oskar.Norberg on 2025-08-27.
//

#include "AssetManager/AssetManager.h"

RNGOEngine::AssetHandling::AssetManager::AssetManager(Core::Renderer::IRenderer& renderer)
    : m_renderer(renderer)
{
}

RNGOEngine::Core::Renderer::MeshHandle RNGOEngine::AssetHandling::AssetManager::CreateMesh(
    std::span<float> vertices, std::span<unsigned int> indices) const
{
    return m_renderer.CreateMesh(vertices, indices);
}

RNGOEngine::Core::Renderer::ShaderHandle RNGOEngine::AssetHandling::AssetManager::CreateShader(
    std::string_view vertexSource, std::string_view fragmentSource) const
{
    return m_renderer.CreateShader(vertexSource, fragmentSource);
}