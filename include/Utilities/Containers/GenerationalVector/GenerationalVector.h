//
// Created by Oskar.Norberg on 2025-10-13.
//

#pragma once

#include <vector>
#include <optional>
#include <functional>
#include <utility>

#include "Utilities/RNGOAsserts.h"
#include "Utilities/Hash/PairHash.h"

namespace RNGOEngine::Containers::Vectors
{
    // Doesn't strictly need to be templated, but improves readability.
    template<typename T>
    struct GenerationalKey
    {
        size_t ID;
        size_t Generation;

        bool operator==(const GenerationalKey<T>& other) const
        {
            return ID == other.ID && Generation == other.Generation;
        }
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

    public:
        static GenerationalKey<T> InvalidKey()
        {
            return {static_cast<size_t>(-1), static_cast<size_t>(-1)};
        }

    private:
        std::vector<InternalGenerationalKey<T>> m_keys;
        std::vector<int> m_freeIndices;
    };

#include "GenerationalVector.tpp"
}

// Hash Function for GenerationalKeys
namespace std
{
    template<typename T>
    struct hash<RNGOEngine::Containers::Vectors::GenerationalKey<T>>
    {
        size_t operator()(const RNGOEngine::Containers::Vectors::GenerationalKey<T>& key) const noexcept
        {
            return RNGOEngine::Utilities::Hash::CombineHashes(std::hash<size_t>{}(key.ID),
                                                              std::hash<size_t>{}(key.Generation));
        }
    };
}