template<typename T, typename T2, typename Hash>
std::optional<T2> AssetCache<T, T2, Hash>::TryGet(const T& key) const
{
    if (const auto it = m_cache.find(key); it != m_cache.end())
    {
        return it->second;
    }

    return std::nullopt;
}

template<typename T, typename T2, typename Hash>
T2 AssetCache<T, T2, Hash>::Get(const T& key) const
{
    return m_cache.at(key);
}

template<typename T, typename T2, typename Hash>
bool AssetCache<T, T2, Hash>::Contains(const T& key) const
{
    return m_cache.contains(key);
}

template<typename T, typename T2, typename Hash>
void AssetCache<T, T2, Hash>::Insert(const T& key, const T2& value)
{
    m_cache[key] = value;
}