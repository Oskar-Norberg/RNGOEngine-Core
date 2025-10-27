//
// Created by Oskar.Norberg on 2025-10-24.
//

#pragma once

#include <functional>
#include <cstdint>

namespace RNGOEngine::Utilities
{
    // NOTE: Could be made into a templated class to support different sizes, but for now just 64 bit.

    // 64-bit wide UUID.
    class UUID
    {
    public:
        UUID();
        explicit UUID(std::uint64_t value);
        UUID(const UUID& other);

        bool operator==(const UUID& other) const;

        explicit constexpr operator uint64_t() const
        {
            return m_uuid;
        }

    private:
        std::uint64_t m_uuid;
    };
}

// Hash Function for UUIDs
namespace std
{
    template<>
    struct hash<RNGOEngine::Utilities::UUID>
    {
        size_t operator()(const RNGOEngine::Utilities::UUID& uuid) const noexcept
        {
            return std::hash<uint64_t>{}(static_cast<uint64_t>(uuid));
        }
    };
}