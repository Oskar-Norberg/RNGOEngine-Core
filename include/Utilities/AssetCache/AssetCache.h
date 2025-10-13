//
// Created by Oskar.Norberg on 2025-09-11.
//

#pragma once

namespace RNGOEngine::Utilities
{
    template<typename T, typename T2, typename Hash = std::hash<T>>
    class AssetCache
    {
    public:
        bool Contains(const T& key) const;
        
        std::optional<T2> TryGet(const T& key) const;
        T2 Get(const T& key) const;

        void Insert(const T& key, const T2& value);
        void Remove(const T& key);

        void Clear();

    private:
        std::unordered_map<T, T2, Hash> m_cache;
    };

#include "Utilities/AssetCache/AssetCache.tpp"
}