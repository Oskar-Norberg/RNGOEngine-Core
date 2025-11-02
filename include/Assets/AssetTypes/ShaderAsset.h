//
// Created by ringo on 2025-10-31.
//

#pragma once
#include "Assets/Asset.h"
#include "Renderer/RenderID.h"

namespace RNGOEngine::AssetHandling
{
    struct ShaderMetadata : AssetMetadata
    {
        Core::Renderer::ShaderType ShaderType;
    };
}