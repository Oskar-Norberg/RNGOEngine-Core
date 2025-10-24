//
// Created by Oskar.Norberg on 2025-10-24.
//

#include "Utilities/UUID/UUID.h"

#include <random>

namespace RNGOEngine::Utilities
{
    static std::random_device s_randomDevice;
    static std::mt19937_64 s_generator(s_randomDevice());
    static std::uniform_int_distribution<uint64_t> s_distribution;

    UUID::UUID()
        : m_uuid(s_distribution(s_generator))
    {
    }

    UUID::UUID(const std::uint64_t value)
        : m_uuid(value)
    {
    }

    UUID::UUID(const UUID& other) = default;

    bool UUID::operator==(const UUID& other) const
    {
        return m_uuid == other.m_uuid;
    }

    constexpr UUID::operator uint64_t() const
    {
        return m_uuid;
    }
}