//
// Created by Oskar.Norberg on 2025-11-24.
//

#pragma once

#include <unordered_map>

#include "Assets/Asset.h"
#include "Assets/AssetTypes/ModelAsset.h"
#include "Assets/AssetTypes/ShaderAsset.h"
#include "Assets/AssetTypes/TextureAsset.h"
#include "Concepts/Concepts.h"
#include "Utilities/Singleton/Singleton.h"

namespace RNGOEngine::AssetHandling
{
    template<typename T>
    using AssetMap = std::unordered_map<AssetHandle, T>;

    class RuntimeAssetRegistry : public Utilities::Singleton<RuntimeAssetRegistry>
    {
    public:
        RuntimeAssetRegistry();

        void Insert(AssetType type, AssetHandle handle, std::unique_ptr<Asset> asset);
        void Remove(AssetType type, AssetHandle handle);

        template<Concepts::DerivedFrom<Asset> TAsset>
        std::optional<std::reference_wrapper<TAsset>> TryGet(AssetHandle handle);

        template<Concepts::DerivedFrom<Asset> TAsset>
        std::optional<std::reference_wrapper<const TAsset>> TryGet(AssetHandle handle) const;

    private:
        // NOTE: These have to correspond with the AssetType enum.
        std::tuple<
            std::monostate,          // Placeholder for None
            AssetMap<ModelAsset>,    // Models
            AssetMap<TextureAsset>,  // Textures
            AssetMap<ShaderAsset>,   // Shaders
            std::monostate,          // TODO: Materials....
            std::monostate           // Placeholder for Count
            >
            m_assetStorage;
    };

#include "RuntimeAssetRegistry.tpp"
}