//
// Created by Oskar.Norberg on 2025-10-13.
//

#pragma once

#include "Utilities/RNGOAsserts.h"
#include <vector>

namespace RNGOEngine::Containers::Vectors
{
    struct GenerationalKey
    {
        int ID;
        int Generation;
    };

    template<typename T>
    struct InternalGenerationalKey
    {
        int ID;
        int Generation;
        T data;
    };

    template<typename T>
    class GenerationalVector
    {
    public:
        GenerationalKey Add(T&& data);
        void Remove(const GenerationalKey& key);

        bool IsValid(const GenerationalKey& key) const;
        const T& Get(const GenerationalKey& key) const;
        T& Get(const GenerationalKey& key);

    private:
        std::vector<InternalGenerationalKey<T>> m_keys;
        std::vector<int> m_freeIndices;
    };

#include "GenerationalVector.tpp"
}