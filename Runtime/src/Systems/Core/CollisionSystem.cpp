//
// Created by ringo on 2026-01-27.
//

#include "Systems/Core/CollisionSystem.h"

#include "Systems/SystemContext.h"
#include "World/World.h"

namespace RNGOEngine::Systems::Core
{
    void CollisionSystem::Update(RNGOEngine::Core::World& world, EngineSystemContext& context)
    {
        EngineSystem::Update(world, context);
        CollisionList collisions;

        // Sphere -> Sphere CD
        auto sphereView = world.GetRegistry().view<Components::Transform, Components::SphereCollider>();

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

        context.GameResourceMapper->AddTransientResource<CollisionList>(std::move(collisions), true);
    }
}