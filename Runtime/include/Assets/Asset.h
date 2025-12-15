//
// Created by Oskar.Norberg on 2025-10-31.
//

#pragma once

#include <filesystem>
#include <utility>

#include "Concepts/Concepts.h"
#include "Utilities/UUID/UUID.h"

namespace RNGOEngine::AssetHandling
{
    // TODO: AssetHandles should store Type.
    using AssetHandle = Utilities::UUID;

    // NOTE: To add new AssetType, add here and create corresponding Metadata struct inheriting from AssetMetadata.
    // Then update AssetMetadataTypes.h to map Metadata to AssetType.
    // Also create corresponding AssetImporter and register it in AssetLoader.
    // Finally, update the AssetFetcher with the new AssetType.
    // TODO: If we are using template values for AssetMetadataTypes, this doesn't really need to exist. Assets can just "be their metadata".
    // And then whatever runtime manager handles them can store the runtime data.
    enum class AssetType
    {
        None,
        Model,
        Texture,
        // TODO: Should Vertex and Fragment shaders be regarded as separate asset types? Or should they share the same AssetType?
        // Or should they be merged into a single "Shader" file that contains both vertex and fragment shader code?
        Shader,
        Material,
        Count
    };
    // TODO: this should probably be replaced with the magic_enum count instead.
    constexpr std::size_t AssetTypeCount = std::to_underlying(AssetType::Count);

    enum class AssetState
    {
        None,
        Ready,
        Invalid,
        Loading
    };

    class Asset
    {
    public:
        explicit Asset(AssetHandle&& handle)
            : m_handle(std::move(handle))
        {
        }

        virtual ~Asset() = default;

    public:
        AssetType GetType() const;
        bool IsType(AssetType type) const;

        // TODO: Unsafe as all hell.
        template<Concepts::DerivedFrom<Asset> TAsset>
        TAsset& GetAsType()
        {
            return static_cast<TAsset&>(*this);
        }

        template<Concepts::DerivedFrom<Asset> TAsset>
        const TAsset& GetAsType() const
        {
            return static_cast<const TAsset&>(*this);
        }

    public:
        AssetState GetState() const;
        bool IsReady() const;

    private:
        AssetHandle m_handle;
    };

    // TODO: Should AssetMetadata really be polymorphic? Or should it just store a Blackboard of key-value pairs?
    struct AssetMetadata
    {
        virtual ~AssetMetadata() = default;

        Utilities::UUID UUID = Utilities::GenerateUUID();
        std::filesystem::path Path = {};
        AssetType Type = AssetType::None;
    };
}