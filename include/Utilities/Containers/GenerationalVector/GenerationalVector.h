//
// Created by Oskar.Norberg on 2025-10-13.
//

#pragma once

#include <vector>
#include "Utilities/RNGOAsserts.h"

namespace RNGOEngine::Containers::Vectors
{
    // Doesn't strictly need to be templated, but improves readability.
    template<typename T>
    struct GenerationalKey
    {
        int ID;
        int Generation;
    };

    template<typename T>
    struct InternalGenerationalKey
    {
        int Generation;
        bool IsLive;
        T data;
    };

    template<typename T>
    class GenerationalVector
    {
    public:
        GenerationalKey<T> Add(T&& data);
        
        void MarkForRemoval(const GenerationalKey<T>& key);
        void Remove(const GenerationalKey<T>& key);

        bool IsValid(const GenerationalKey<T>& key) const;
        const T& Get(const GenerationalKey<T>& key) const;
        T& Get(const GenerationalKey<T>& key);

        auto begin()
        {
            return m_keys.begin();
        }

        auto end()
        {
            return m_keys.end();
        }

        auto begin() const
        {
            return m_keys.begin();
        }

        auto end() const
        {
            return m_keys.end();
        }

    private:
        std::vector<InternalGenerationalKey<T>> m_keys;
        std::vector<int> m_freeIndices;
    };

#include "GenerationalVector.tpp"
}