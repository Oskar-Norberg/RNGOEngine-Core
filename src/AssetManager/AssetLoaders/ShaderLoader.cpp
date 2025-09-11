//
// Created by Oskar.Norberg on 2025-09-03.
//

#include "AssetManager/AssetLoaders/ShaderLoader.h"

namespace RNGOEngine::AssetHandling
{
    std::string ShaderLoader::LoadShader(const std::filesystem::path& path) const
    {
        return shaderPreprocessor.Parse(path);
    }
}