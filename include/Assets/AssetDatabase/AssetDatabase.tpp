template<Concepts::DerivedFrom<AssetMetadata> TMetadata>
TMetadata& AssetDatabase::RegisterAsset(AssetType type, const std::filesystem::path& assetPath)
{
    const auto uuid = Utilities::UUID();
    auto metadata = std::make_unique<TMetadata>();
    // Set common metadata fields. Consumer sets type-specifics.
    // TODO: ^ Should the templated function just take an argument list?
    // TODO ^^ Probably yes. Please do.
    metadata->UUID = uuid;
    metadata->Path = assetPath;
    metadata->State = AssetState::Invalid; // State is driven by the Runtime Manager. Invalid on register.
    metadata->Type = type;

    m_assetMetadataMap.insert({uuid, std::move(metadata)});
    m_pathToHandle.insert({assetPath, uuid});
    return *static_cast<TMetadata*>(m_assetMetadataMap.at(uuid).get());
}