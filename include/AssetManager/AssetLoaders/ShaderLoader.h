//
// Created by Oskar.Norberg on 2025-09-03.
//

#pragma once

#include "AssetManager/AssetFileFetcher.h"
#include "Renderer/IRenderer.h"
#include "Shader/ShaderPreprocessor.h"

namespace RNGOEngine::AssetHandling
{
    class ShaderLoader
    {
    public:
        ShaderLoader(Core::Renderer::IRenderer& renderer, AssetFileFetcher& assetFetcher)
            : shaderPreprocessor(assetFetcher), m_renderer(renderer), m_assetFileFetcher(assetFetcher)
        {
        }

        Core::Renderer::ShaderID LoadShader(const std::filesystem::path& path, Core::Renderer::ShaderType type) const;
        Core::Renderer::ShaderProgramID CreateShaderProgram(Core::Renderer::ShaderID vertex, Core::Renderer::ShaderID fragment) const;

    private:
        Shaders::ShaderPreProcessor shaderPreprocessor;

        Core::Renderer::IRenderer& m_renderer;
        AssetFileFetcher& m_assetFileFetcher;
    };
}