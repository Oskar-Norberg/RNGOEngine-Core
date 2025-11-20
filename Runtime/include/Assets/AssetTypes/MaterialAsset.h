//
// Created by ringo on 2025-10-31.
//

#pragma once

#include "Assets/Asset.h"
// TODO: SHit ass include path, the material should not inherently be tied to the system that manages it.
#include "Assets/AssetManager/Managers/Material/Material.h"

namespace RNGOEngine::AssetHandling
{
    struct MaterialMetadata : AssetMetadata
    {
        Utilities::UUID VertexShader;
        Utilities::UUID FragmentShader;
        MaterialParameters Parameters;
    };
}