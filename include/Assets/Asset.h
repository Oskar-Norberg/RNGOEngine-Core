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

    enum class AssetState
    {
        None,
        Invalid,
        Valid
    };

    struct AssetMetadata
    {
        Utilities::UUID UUID;
        std::filesystem::path Path;
        AssetState State = AssetState::None;
        AssetType Type = AssetType::None;
    };

    // TODO: For now there is no Asset base class. Assets can be whatever their corresponding Runtime Manager wants them to be.
    // This might need to change in the future.
}