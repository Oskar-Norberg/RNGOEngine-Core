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