//
// Created by Oskar.Norberg on 2025-09-03.
//

#include "Assets/AssetLoaders/ShaderLoader.h"

#include "Data/Shaders/ShaderSpecification.h"

namespace RNGOEngine::AssetHandling
{
    ShaderLoader::ShaderLoader()
        : shaderPreprocessor(Data::Shader::RNGO_SHADER_DEFINITIONS)
    {
    }

    std::expected<Shaders::ShaderParseResult, Shaders::ShaderPreProcessingError> ShaderLoader::LoadShader(
        const std::filesystem::path& path
    ) const
    {
        return shaderPreprocessor.Parse(path);
    }
}