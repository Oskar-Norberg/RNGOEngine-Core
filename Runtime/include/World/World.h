//
// Created by SkummisDenAndre on 2025-06-16.
//

#pragma once

#include <entt/entt.hpp>

#include "Components/Components.h"
#include "ECS/Entity/Entity.h"

namespace RNGOEngine::Core
{
    class World
    {
    public:
        // TODO: not being able to pass it by reference is annoying.
        Entity CreateEntity(Components::Name name, Components::Transform transform);
        Entity CreateEntity(Components::Name name);
        Entity CreateEntity(Components::Transform transform);
        Entity CreateEntity();

    public:
        void DestroyEntity(entt::entity entity);

        // TODO: The goal is for no one to access the registry directly. Accessing only through the Entity/World wrappers. But for now, this is good enough.
        entt::registry& GetRegistry()
        {
            return m_registry;
        }

        // Gravity
    public:
        float GetGravity() const;
        void SetGravity(float gravity);

    private:
        float m_gravity = 9.82f;
        entt::registry m_registry;
    };
}