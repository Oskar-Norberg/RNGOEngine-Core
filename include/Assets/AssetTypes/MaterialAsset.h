//
// Created by ringo on 2025-10-31.
//

#pragma once
#include "Assets/Asset.h"

namespace RNGOEngine::AssetHandling
{
    struct MaterialMetadata : AssetMetadata
    {
        Utilities::UUID vertexShader;
        Utilities::UUID fragmentShader;
        MaterialParameters parameters;
    };
}