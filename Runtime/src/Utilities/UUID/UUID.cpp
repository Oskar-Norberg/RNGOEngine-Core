//
// Created by Oskar.Norberg on 2025-10-24.
//

#include "Utilities/UUID/UUID.h"

namespace RNGOEngine::Utilities
{
    UUID::UUID(const std::uint64_t value)
        : m_uuid(value)
    {
    }

    UUID::UUID()
        : UUID(0)
    {
    }


    UUID::~UUID()
    {
    }

    UUID& UUID::operator=(const UUID& other)
    {
        m_uuid = other.m_uuid;
        return *this;
    }

    UUID::UUID(UUID&& other) noexcept
    {
        m_uuid = other.m_uuid;
    }

    UUID& UUID::operator=(UUID&& other) noexcept
    {
        m_uuid = other.m_uuid;
        return *this;
    }

    UUID::UUID(const UUID& other) = default;

    bool UUID::operator==(const UUID& other) const
    {
        return m_uuid == other.m_uuid;
    }
}