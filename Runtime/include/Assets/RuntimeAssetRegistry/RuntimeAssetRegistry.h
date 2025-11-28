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
    class AssetRegistryEntry
    {
    public:
        virtual ~AssetRegistryEntry() = default;

        virtual AssetState GetState() const = 0;
        virtual void SetState(AssetState state) = 0;
    };

    template<Concepts::DerivedFrom<Asset> TAsset>
    class AssetRegistryEntryT : public AssetRegistryEntry
    {
    public:
        AssetState State;
        TAsset Asset;

        AssetRegistryEntryT(const AssetState state, TAsset&& asset)
            : State(state), Asset(std::move(asset))
        {
        }

    public:
        AssetState GetState() const override
        {
            return State;
        }

        void SetState(const AssetState state) override
        {
            State = state;
        }
    };

    template<Concepts::DerivedFrom<Asset> TAsset>
    using AssetMap = std::unordered_map<AssetHandle, AssetRegistryEntryT<TAsset>>;

    class RuntimeAssetRegistry : public Utilities::Singleton<RuntimeAssetRegistry>
    {
    public:
        RuntimeAssetRegistry();

        template<Concepts::DerivedFrom<Asset> TAsset>
        AssetRegistryEntry& Insert(const AssetHandle& handle, TAsset&& asset)
        {
            // TODO: Make Static Assert Macro
            static_assert(
                std::is_same_v<TAsset, ModelAsset> || std::is_same_v<TAsset, TextureAsset> ||
                    std::is_same_v<TAsset, ShaderAsset>,
                "RuntimeAssetRegistry::Insert - Unsupported asset type."
            );
            auto& storage = std::get<AssetMap<TAsset>>(m_assetStorage);

            const auto it = storage.emplace(
                AssetHandle(handle), AssetRegistryEntryT(AssetState::Invalid, std::move(asset))
            );
            return it.first->second;
        }

        void Remove(AssetType type, AssetHandle handle);

        AssetState GetState(AssetType type, const AssetHandle& handle) const;
        void SetState(AssetType type, const AssetHandle& handle, AssetState state);

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