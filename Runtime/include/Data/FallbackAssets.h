//
// Created by Oskar.Norberg on 2025-11-17.
//

#pragma once

#include <filesystem>

// Paths to all fallback assets.
namespace RNGOEngine::Data::FallbackAssets
{
    const std::filesystem::path InvalidModel = "fallbacks/InvalidModel.obj";
    const std::filesystem::path InvalidTexture = "fallbacks/InvalidTexture.png";

    const std::filesystem::path InvalidVertexShader = "fallbacks/InvalidShader.vert";
    const std::filesystem::path InvalidFragmentShader = "fallbacks/InvalidShader.frag";
}
