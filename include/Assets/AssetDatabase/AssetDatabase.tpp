#include "Utilities/RNGOAsserts.h"

template<Concepts::DerivedFrom<AssetMetadata> TMetadata>
AssetHandle AssetDatabase::RegisterAsset(const std::filesystem::path& assetPath)
{
    // TODO: Custom wrapper for static_asserts
    static_assert(
        requires { AssetTypeForMetadata<TMetadata>::value; } &&
            requires { static_cast<AssetType>(AssetTypeForMetadata<TMetadata>::value); },
        "AssetDatabase::RegisterAsset Metadata Type has no corresponding AssetType defined. Add it to "
        "Assets/AssetMetadataTypes.h"
    );

    if (!m_metadataStorages.contains(AssetTypeForMetadata<TMetadata>::value))
    {
        m_metadataStorages.insert(
            {AssetTypeForMetadata<TMetadata>::value, std::make_unique<AssetMetadataStorageTyped<TMetadata>>()}
        );
    }

    auto& storageTyped = static_cast<AssetMetadataStorageTyped<TMetadata>&>(
        *m_metadataStorages.at(AssetTypeForMetadata<TMetadata>::value)
    );

    const auto uuid = Utilities::UUID();

    TMetadata metadata;
    // Set common metadata fields. Consumer sets type-specifics.
    metadata.UUID = uuid;
    metadata.Path = assetPath;
    metadata.State = AssetState::Invalid;  // State is driven by the Runtime Manager. Invalid on register.
    metadata.Type = AssetTypeForMetadata<TMetadata>::value;

    const auto index = storageTyped.Storage.Insert(std::move(metadata));
    m_handleToStorageIndex.insert({uuid, std::make_pair(AssetTypeForMetadata<TMetadata>::value, index)});

    m_pathToHandle.insert({assetPath, uuid});

    return uuid;
}

template<Concepts::DerivedFrom<AssetMetadata> TMetadata>
void AssetDatabase::UnregisterAsset(const AssetHandle& uuid)
{
    if (m_handleToStorageIndex.contains(uuid))
    {
        const auto& storageIndex = m_handleToStorageIndex.at(uuid);
        auto& storageTyped = static_cast<AssetMetadataStorageTyped<TMetadata>&>(
            *m_metadataStorages.at(AssetTypeForMetadata<TMetadata>::value)
        );

        const auto* metadataPtr = storageTyped.Storage.Get(storageIndex.second);
        RNGO_ASSERT(metadataPtr && "AssetDatabase::UnregisterAsset - Metadata pointer is null.");

        const auto& path = metadataPtr->Path;
        m_pathToHandle.erase(path);
        m_handleToStorageIndex.erase(uuid);
        storageTyped.Storage.Remove(storageIndex.second);
    }
}

template<Concepts::DerivedFrom<AssetMetadata> TMetadata>
TMetadata& AssetDatabase::GetAssetMetadataAs(const AssetHandle& handle)
{
    return const_cast<TMetadata&>(
        static_cast<const AssetDatabase*>(this)->GetAssetMetadataAs<TMetadata>(handle)
    );
}

template<Concepts::DerivedFrom<AssetMetadata> TMetadata>
const TMetadata& AssetDatabase::GetAssetMetadataAs(const AssetHandle& handle) const
{
    // REALLY UNSAFE DOWNCAST.
    const auto& metadata = GetAssetMetadata(handle);
    RNGO_ASSERT(
        metadata.Type == AssetTypeForMetadata<TMetadata>::value &&
        "AssetDatabase::GetAssetMetadataAs<TMetadata> - AssetType does not match requested Metadata type."
    );

    return static_cast<const TMetadata&>(metadata);
}

template<Concepts::DerivedFrom<AssetMetadata> TMetadata>
std::optional<std::reference_wrapper<TMetadata>> AssetDatabase::TryGetAssetMetadataAs(
    const AssetHandle& handle
)
{
    const auto constThis = static_cast<const AssetDatabase*>(this);
    if (const auto assetMetadataOpt = constThis->TryGetAssetMetadataAs<TMetadata>(handle); assetMetadataOpt)
    {
        return std::ref(*const_cast<TMetadata&>(assetMetadataOpt->get()));
    }

    return std::nullopt;
}

template<Concepts::DerivedFrom<AssetMetadata> TMetadata>
std::optional<std::reference_wrapper<const TMetadata>> AssetDatabase::TryGetAssetMetadataAs(
    const AssetHandle& handle
) const
{
    if (const auto metaDataOpt = TryGetAssetMetadata(handle); metaDataOpt)
    {
        return std::cref(static_cast<const TMetadata&>(*metaDataOpt));
    }

    return std::nullopt;
}