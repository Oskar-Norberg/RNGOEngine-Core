//
// Created by SkummisDenAndre on 2025-06-16.
//

#pragma once

#include "entt.hpp"

// Entity wrapper class.
// This is just a handle, not the entity itself.
struct Entity
{
    ~Entity() = default;
    Entity() = delete;
    Entity(const Entity& other) = default;
    Entity& operator=(const Entity& other);

    template<typename T>
    void AddComponent(const T& component);

    template<typename T>
    T& GetComponent();

    template<typename T>
    bool TryGetComponent(T& outComponent);

    ///
    /// Please don't ever use this function.
    /// @return Raw entt registry.
    ///
    entt::registry& GetRegistry() const { return m_registry; }

    ///
    /// Please don't ever use this function.
    /// @return Raw entt registry.
    ///
    entt::registry& GetRegistry() { return m_registry; }

private:
    friend class World;

    explicit Entity(entt::entity entity, entt::registry& registry)
        : m_entity(entity), m_registry(registry) {}

    entt::entity m_entity;
    entt::registry& m_registry;
};

#include "Entity.tpp"