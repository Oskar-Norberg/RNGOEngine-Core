//
// Created by Oskar.Norberg on 2025-09-10.
//

#include "Assets/AssetManager/Managers/MaterialManager.h"

#include "Assets/RuntimeAssetRegistry/RuntimeAssetRegistry.h"

namespace RNGOEngine::AssetHandling
{
    MaterialAsset& MaterialManager::CreateMaterial(
        const AssetHandle& handle, const MaterialSpecification& materialSpecification
    )
    {
        MaterialAsset materialAsset({handle.UUID, AssetType::Material}, materialSpecification);

        auto& runtimeRegistry = RuntimeAssetRegistry::GetInstance();
        auto& entry = runtimeRegistry.Insert<MaterialAsset>(handle, std::move(materialAsset));
        entry.SetState(AssetState::Ready);

        auto& insertedMaterialAsset = runtimeRegistry.TryGet<MaterialAsset>(handle)->get();

        return insertedMaterialAsset;
    }
}