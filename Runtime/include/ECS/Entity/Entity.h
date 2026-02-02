//
// Created by Oskar.Norberg on 2025-11-13.
//

#pragma once

#include <entt/entt.hpp>

namespace RNGOEngine
{
    namespace Core
    {
        class World;
    }
}

namespace RNGOEngine::Core
{
    class Entity
    {
    public:
        Entity() = delete;
        Entity& operator=(Entity& other) = delete;

    public:
        template<typename T, typename... Args>
        auto AddComponent(Args&&... args) -> decltype(auto);

        template<typename T>
        T& GetComponent();

        template<typename T>
        bool HasComponent() const;

        template<typename T>
        void RemoveComponent();

        entt::entity GetID() const
        {
            return m_id;
        }

    private:
        Entity(const entt::entity id, entt::registry& registry)
            : m_id(id), m_registry(registry)
        {
        }

    private:
        friend World;

    private:
        const entt::entity m_id;
        entt::registry& m_registry;
    };

#include "Entity.inl"
}