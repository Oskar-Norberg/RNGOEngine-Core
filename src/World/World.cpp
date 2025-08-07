//
// Created by Oskar.Norberg on 2025-06-17.
//

#include "World/World.h"

entt::entity World::CreateEntity()
{
    return m_registry.create();
}

void World::DestroyEntity(entt::entity entity)
{
    m_registry.destroy(entity);
}
