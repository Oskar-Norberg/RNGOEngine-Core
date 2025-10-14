//
// Created by Oskar.Norberg on 2025-10-13.
//

#pragma once

#include <vector>
#include <optional>
#include <functional>
#include <utility>

#include "Utilities/RNGOAsserts.h"

namespace RNGOEngine::Containers::Vectors
{
    // Doesn't strictly need to be templated, but improves readability.
    template<typename T>
    struct GenerationalKey
    {
        size_t ID;
        size_t Generation;
    };

    template<typename T>
    struct InternalGenerationalKey
    {
        size_t Generation;
        bool IsLive;
        T Data;
    };

    template<typename T>
    class GenerationalVector
    {
    public:
        template<typename U>
        GenerationalKey<T> Insert(U&& data);

        void MarkForRemoval(const GenerationalKey<T>& key);
        void Remove(const GenerationalKey<T>& key);

        bool IsValid(const GenerationalKey<T>& key) const;
        const T& Get(const GenerationalKey<T>& key) const;
        T& Get(const GenerationalKey<T>& key);

        std::optional<std::reference_wrapper<const T>> GetValidated(const GenerationalKey<T>& key) const;
        std::optional<std::reference_wrapper<T>> GetValidated(const GenerationalKey<T>& key);

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