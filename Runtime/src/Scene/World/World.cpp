//
// Created by Oskar.Norberg on 2025-06-17.
//

#include "Scene/World/World.h"

#include "ECS/Components/Components.h"

namespace RNGOEngine::Core
{
    Entity World::CreateEntity(const Components::Name name, const Components::Transform transform)
    {
        const auto entityID = m_registry.create();

        auto entity = Entity(entityID, m_registry);
        entity.AddComponent<Components::Name>(name);
        entity.AddComponent<Components::Transform>(transform);

        return entity;
    }

    Entity World::CreateEntity(const Components::Name name)
    {
        return CreateEntity(name, Components::Transform{});
    }

    Entity World::CreateEntity(const Components::Transform transform)
    {
        return CreateEntity(Components::Name{}, transform);
    }

    Entity World::CreateEntity()
    {
        return CreateEntity(Components::Name{});
    }

    void World::DestroyEntity(const entt::entity entity)
    {
        m_registry.destroy(entity);
    }

    glm::vec3 World::GetGravity() const
    {
        return m_gravity;
    }

    void World::SetGravity(const glm::vec3 gravity)
    {
        m_gravity = gravity;
    }
}
