//
// Created by Oskar.Norberg on 2025-10-31.
//

#pragma once

#include "Utilities/UUID/UUID.h"

namespace RNGOEngine::AssetHandling
{
    using AssetHandle = Utilities::UUID;

    // NOTE: To add new AssetType, add here and create corresponding Metadata struct inheriting from AssetMetadata.
    // Then update AssetMetadataTypes.h to map Metadata to AssetType.
    // TODO: If we are using template values for AssetMetadataTypes, this doesn't really need to exist. Assets can just "be their metadata".
    // And then whatever runtime manager handles them can store the runtime data.
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
        // TODO: Would be nice to have a runtime link to the data here. But that would require a base Asset class along with a Ref counted pointer system.
        // This means either everything has to be shared_ptrs (cache coherency be damned) or implementing a ref counting system.
    };

    // TODO: For now there is no Asset base class. Assets can be whatever their corresponding Runtime Manager wants them to be.
    // This might need to change in the future.
}