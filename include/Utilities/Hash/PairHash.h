//
// Created by ringo on 2025-08-16.
//

#pragma once

#include <cstddef>
#include <cstdint>
#include <functional>
#include <utility>

namespace RNGOEngine::Utilities::Hash
{
    inline uint64_t PackUint32Pair(uint32_t a, uint32_t b)
    {
        return (static_cast<uint64_t>(a) << 32) | static_cast<uint64_t>(b);
    }
    
    struct PairHash
    {
        template<typename T1, typename T2>
        std::size_t operator()(const std::pair<T1, T2>& p) const noexcept
        {
            std::size_t h1 = std::hash<T1>{}(p.first);
            std::size_t h2 = std::hash<T2>{}(p.second);
            
            return h1 ^ (h2 << 1);
        }
    };
}