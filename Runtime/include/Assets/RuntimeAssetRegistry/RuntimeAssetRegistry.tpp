template<Concepts::DerivedFrom<Asset> TAsset>
TAsset* RuntimeAssetRegistry::Get(AssetHandle handle)
{
    const auto& assetMap = std::get<AssetMap<TAsset>>(m_assetStorage);
    auto it = assetMap.find(handle);

    if (it != assetMap.end())
    {
        return &it->second;
    }

    return nullptr;
}