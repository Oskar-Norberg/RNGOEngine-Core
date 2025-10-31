//
// Created by Oskar.Norberg on 2025-09-03.
//

#pragma once

#include "Assets/AssetFetcher/AssetFetcher.h"
#include "Shader/ShaderPreprocessor.h"

namespace RNGOEngine::AssetHandling
{
    class ShaderLoader
    {
    public:
        explicit ShaderLoader(const AssetFetcher& assetFetcher)
            : shaderPreprocessor(assetFetcher)
        {
        }

        std::expected<std::string, Shaders::ShaderPreProcessingError> LoadShader(const std::filesystem::path& path) const;

    private:
        Shaders::ShaderPreProcessor shaderPreprocessor;
    };
}