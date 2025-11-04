//
// Created by Oskar.Norberg on 2025-09-03.
//

#include "Assets/AssetLoaders/ShaderLoader.h"

namespace RNGOEngine::AssetHandling
{
    std::expected<std::string, Shaders::ShaderPreProcessingError> ShaderLoader::LoadShader(const std::filesystem::path& path) const
    {
        return shaderPreprocessor.Parse(path);
    }
}