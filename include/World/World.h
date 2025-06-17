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

private:
    entt::registry m_registry;
};
