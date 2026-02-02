//
// Created by ringo on 2026-01-27.
//

#include "ECS/Systems/Core/Physics/CollisionSystem.h"

#include "ECS/Systems/SystemContext.h"
#include "Scene/World/World.h"

namespace RNGOEngine::Systems::Core
{
    void CollisionSystem::Update(RNGOEngine::Core::World& world, EngineSystemContext& context)
    {
        EngineSystem::Update(world, context);
        CollisionList collisions = {};

        // Sphere -> Sphere CD
        const auto sphereView = world.GetRegistry().view<Components::Transform, Components::SphereCollider>();

        // Simple O(n^2) for now
        for (const auto& [entityA, transformA, sphereA] : sphereView.each())
        {
            for (const auto& [entityB, transformB, sphereB] : sphereView.each())
            {
                if (entityA >= entityB)
                {
                    continue;
                }

                if (glm::length(transformA.Position - transformB.Position) <= sphereA.Radius + sphereB.Radius)
                {
                    collisions.collisions.emplace_back(entityA, entityB);
                }
            }
        }

        // Register collisions for both engine and game systems. This will copy the data, but it's fine for now.
        // TODO: Consider using a shared pointer if this becomes a problem.
        context.EngineResourceMapper->AddTransientResource(CollisionList{collisions}, true);
        context.GameResourceMapper->AddTransientResource(std::move(collisions), true);
    }
}