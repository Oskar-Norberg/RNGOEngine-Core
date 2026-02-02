//
// Created by Oskar.Norberg on 2025-10-31.
//

#pragma once

#include <filesystem>
#include <utility>

#include "Utilities/Concepts/Concepts.h"
#include "Utilities/Hash/PairHash.h"
#include "Utilities/UUID/UUID.h"

namespace RNGOEngine::AssetHandling
{
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

    struct AssetHandle
    {
        Utilities::UUID UUID;
        AssetType Type = AssetType::None;

        bool operator==(const AssetHandle&) const = default;
    };

    template<AssetType T>
    struct AssetHandleT : AssetHandle
    {
        static constexpr AssetType TypeValue = T;

        AssetHandleT()
            : AssetHandle{{}, T}
        {
        }

        explicit AssetHandleT(const Utilities::UUID& uuid)
            : AssetHandle{uuid, T}
        {
        }
    };

    using ModelHandle = AssetHandleT<AssetType::Model>;
    using TextureHandle = AssetHandleT<AssetType::Texture>;
    using ShaderHandle = AssetHandleT<AssetType::Shader>;
    using MaterialHandle = AssetHandleT<AssetType::Material>;

    struct AssetHandleHasher
    {
        std::size_t operator()(const AssetHandle& assetHandle) const
        {
            const std::size_t h1 = std::hash<Utilities::UUID>()(assetHandle.UUID);
            const std::size_t h2 =
                std::hash<std::underlying_type_t<AssetType>>()(std::to_underlying(assetHandle.Type));
            return Utilities::Hash::CombineHashes(h1, h2);
        }
    };

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

        const AssetHandle& GetHandle() const
        {
            return m_handle;
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