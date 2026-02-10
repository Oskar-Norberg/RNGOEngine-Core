//
// Created by Oskar.Norberg on 2025-09-10.
//

#pragma once

#include "Assets/AssetTypes/MaterialAsset.h"
#include "Material/Material.h"

namespace RNGOEngine::AssetHandling
{
    class MaterialManager
    {
    public:
        // CreateMaterial and insert it to RuntimeAssetRegistry.
        MaterialAsset& CreateMaterial(
            const AssetHandle& handle, const MaterialSpecification& materialSpecification
        );

        // TODO: Create without inserting to registry?
    };
}