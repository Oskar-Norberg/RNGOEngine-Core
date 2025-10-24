//
// Created by Oskar.Norberg on 2025-10-24.
//

#pragma once

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

        explicit constexpr operator uint64_t() const;

    private:
        std::uint64_t m_uuid;
    };
}