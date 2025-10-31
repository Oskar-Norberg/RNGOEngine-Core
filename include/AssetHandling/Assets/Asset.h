//
// Created by Oskar.Norberg on 2025-10-31.
//

#pragma once

#include "Utilities/UUID/UUID.h"

namespace RNGOEngine::AssetHandling
{
    using AssetHandle = Utilities::UUID;

    enum class AssetType
    {
        None,
        Model,
        Texture,
        Shader,
        Material,
    };
    
    class Asset
    {
        AssetHandle handle;
        AssetType type = AssetType::None;
    };
}