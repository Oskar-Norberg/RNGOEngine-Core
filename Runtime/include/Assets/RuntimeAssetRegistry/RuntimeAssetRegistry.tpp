template<Concepts::DerivedFrom<Asset> TAsset>
std::optional<std::reference_wrapper<TAsset>> RuntimeAssetRegistry::TryGet(AssetHandle handle)
{
    const RuntimeAssetRegistry* constThis = this;
    auto optConst = constThis->TryGet<TAsset>(handle);

    if (!optConst)
    {
        return std::nullopt;
    }

    TAsset* nonConstPtr = const_cast<TAsset*>(&optConst->get());
    return std::ref(*nonConstPtr);
}

template<Concepts::DerivedFrom<Asset> TAsset>
std::optional<std::reference_wrapper<const TAsset>> RuntimeAssetRegistry::TryGet(AssetHandle handle) const
{
    const auto& assetMap = std::get<AssetMap<TAsset>>(m_assetStorage);
    const auto it = assetMap.find(handle);

    if (it != assetMap.end())
    {
        if (it->second.State == AssetState::Ready)
        {
            return std::cref(it->second.Asset);
        }
    }

    return std::nullopt;
}
