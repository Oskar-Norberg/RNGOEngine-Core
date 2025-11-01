template<Concepts::DerivedFrom<AssetMetadata> TMetadata>
AssetHandle AssetDatabase::RegisterAsset(const std::filesystem::path& assetPath)
{
    // TODO: Custom wrapper for static_asserts
    static_assert(
        requires { AssetTypeForMetadata<TMetadata>::value; } &&
        requires { static_cast<AssetType>(AssetTypeForMetadata<TMetadata>::value); },
        "AssetDatabase::RegisterAsset Metadata Type has no corresponding AssetType defined. Add it to Assets/AssetMetadataTypes.h"
    );

    const auto uuid = Utilities::UUID();
    
    auto metadata = std::make_unique<TMetadata>();
    // Set common metadata fields. Consumer sets type-specifics.
    metadata->UUID = uuid;
    metadata->Path = assetPath;
    metadata->State = AssetState::Invalid; // State is driven by the Runtime Manager. Invalid on register.
    metadata->Type = AssetTypeForMetadata<TMetadata>::value;
    
    m_assetMetadataMap.insert({uuid, std::move(metadata)});
    m_pathToHandle.insert({assetPath, uuid});

    return uuid;
}

template<Concepts::DerivedFrom<AssetMetadata> TMetadata>
void AssetDatabase::UnregisterAsset(const AssetHandle& uuid)
{
    if (m_assetMetadataMap.contains(uuid))
    {
        const auto& path = m_assetMetadataMap.at(uuid)->Path;
        m_pathToHandle.erase(path);
        m_assetMetadataMap.erase(uuid);
    }
}