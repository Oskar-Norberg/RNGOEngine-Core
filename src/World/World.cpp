//
// Created by Oskar.Norberg on 2025-06-17.
//

#include "World/World.h"

Entity World::CreateEntity()
{
    auto entity = m_registry.create();
    return Entity(entity, m_registry);
}

void World::DestroyEntity(const Entity& entity)
{
    m_registry.destroy(entity.m_entity);
}
