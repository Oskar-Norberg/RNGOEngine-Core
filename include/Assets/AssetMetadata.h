//
// Created by Oskar.Norberg on 2025-10-31.
//

#pragma once

#include "Utilities/UUID/UUID.h"

namespace RNGOEngine::AssetHandling
{
    enum class AssetState
    {
        None,
        Invalid,
        Valid,
    };
    
    class AssetMetadata
    {
        Utilities::UUID uuid;
        std::filesystem::path path;
        AssetState state = AssetState::None;
    };
}
