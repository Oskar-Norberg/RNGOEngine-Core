//
// Created by Oskar.Norberg on 2025-11-17.
//

#pragma once

#include <filesystem>

// Paths to all fallback assets.
namespace RNGOEngine::Data::Assets
{
    struct DefaultAsset
    {
        AssetHandling::AssetType Type;
        std::filesystem::path Path;
    };

    // Invalid Fallbacks
    const DefaultAsset InvalidModel = {AssetHandling::AssetType::Model, "fallbacks/InvalidModel.obj"};
    const DefaultAsset InvalidTexture = {AssetHandling::AssetType::Texture, "fallbacks/InvalidTexture.png"};
    const DefaultAsset InvalidShader = {AssetHandling::AssetType::Shader, "fallbacks/InvalidShader.glsl"};

    // Builtin Assets
    const DefaultAsset SphereModel = {AssetHandling::AssetType::Model, "builtin/UnitSphere.fbx"};
    const DefaultAsset Cube = {AssetHandling::AssetType::Model, "builtin/UnitCube.obj"};
    const DefaultAsset WireframeShader = {AssetHandling::AssetType::Shader, "builtin/Wireframe.glsl"};

    const static DefaultAsset AllBuiltinAssets[] = {
        InvalidModel, InvalidTexture, InvalidShader,

        SphereModel, Cube, WireframeShader,
    };
}