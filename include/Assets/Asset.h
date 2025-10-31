//
// Created by Oskar.Norberg on 2025-10-31.
//

#pragma once

#include "AssetMetadata.h"
#include "Concepts/Concepts.h"
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

    template<Concepts::DerivedFrom<AssetMetadata> TMetadata>
    class Asset
    {
    public:
        explicit Asset(const AssetType type)
            : type(type)
        {
        }

        Asset() = delete;
        
        virtual ~Asset() = default;

        const AssetHandle& GetHandle() const
        {
            return handle;
        }

        AssetType GetType() const
        {
            return type;
        }

    protected:
        AssetHandle handle;
        AssetType type = AssetType::None;
    };
}