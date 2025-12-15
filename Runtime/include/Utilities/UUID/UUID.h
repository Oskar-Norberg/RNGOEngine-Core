//
// Created by Oskar.Norberg on 2025-10-24.
//

#pragma once

#include <cstdint>
#include <functional>
#include <random>

namespace RNGOEngine::Utilities
{
    // NOTE: Could be made into a templated class to support different sizes, but for now just 64 bit.

    // 64-bit wide UUID.
    class UUID
    {
    public:
        explicit UUID(std::uint64_t value);
        // NOTE: Default constructs to zero UUID. Make sure this is your intention. Use GenerateUUID() to get a random UUID.
        UUID();

        ~UUID();
        UUID(const UUID& other);
        UUID& operator=(const UUID& other);
        UUID(UUID&& other) noexcept;
        UUID& operator=(UUID&& other) noexcept;

        bool operator==(const UUID& other) const;

        std::uint64_t GetValue() const
        {
            return m_uuid;
        }

        explicit constexpr operator uint64_t() const
        {
            return m_uuid;
        }

    private:
        std::uint64_t m_uuid;
    };

    static UUID GenerateUUID()
    {
        static std::random_device s_randomDevice;
        static std::mt19937_64 s_generator(s_randomDevice());
        static std::uniform_int_distribution<uint64_t> s_distribution;

        return UUID(s_distribution(s_generator));
    }
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