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
        explicit ShaderLoader(const AssetFileFetcher& assetFetcher)
            : shaderPreprocessor(assetFetcher)
        {
        }

        std::expected<std::string, Shaders::ShaderPreProcessingError> LoadShader(const std::filesystem::path& path) const;

    private:
        Shaders::ShaderPreProcessor shaderPreprocessor;
    };
}