//
// Created by SkummisDenAndre on 2025-06-16.
//

#pragma once

#include <entt.hpp>
#include "Entity/Entity.h"

class World
{
public:
    Entity CreateEntity();

    void DestroyEntity(const Entity& entity);

    // TODO: The goal is for no one to access the registry directly. Accessing only through the Entity/World wrappers. But for now, this is good enough.
    entt::registry& GetRegistry() { return m_registry; }

private:
    entt::registry m_registry;
};
