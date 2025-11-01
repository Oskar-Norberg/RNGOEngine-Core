//
// Created by ringo on 2025-11-01.
//

#pragma once

#include "Asset.h"
#include "Concepts/Concepts.h"
#include "Utilities/Containers/FreeListVector/FreeListVector.h"

namespace RNGOEngine::AssetHandling
{
    struct AssetMetadataStorage
    {
        virtual ~AssetMetadataStorage() = default;
        virtual AssetMetadata* GetAssetMetadata(size_t index) = 0;
    };

    template<Concepts::DerivedFrom<AssetMetadata> T>
    struct AssetMetadataStorageTyped : AssetMetadataStorage
    {
        Containers::FreeListVector<T> Storage;

        AssetMetadata* GetAssetMetadata(size_t index) override
        {
            return Storage.Get(index);
        }
    };
}