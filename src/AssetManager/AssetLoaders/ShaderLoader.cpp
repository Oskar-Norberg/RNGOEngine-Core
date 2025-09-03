//
// Created by Oskar.Norberg on 2025-09-03.
//

#include "AssetManager/AssetLoaders/ShaderLoader.h"

namespace RNGOEngine::AssetHandling
{
    Core::Renderer::ShaderID ShaderLoader::LoadShader(const std::filesystem::path& path,
                                                      const Core::Renderer::ShaderType type) const
    {
        const auto processedSource = shaderPreprocessor.Parse(path);
        return m_renderer.CreateShader(processedSource, type);
    }

    Core::Renderer::ShaderProgramID ShaderLoader::CreateShaderProgram(Core::Renderer::ShaderID vertex,
                                                                      Core::Renderer::ShaderID fragment) const
    {
        return m_renderer.CreateShaderProgram(vertex, fragment);
    }
}