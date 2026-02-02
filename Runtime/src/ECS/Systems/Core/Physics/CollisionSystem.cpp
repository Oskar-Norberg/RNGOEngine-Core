//
// Created by ringo on 2026-01-27.
//

#include "ECS/Systems/Core/Physics/CollisionSystem.h"

#include "ECS/Systems/SystemContext.h"
#include "Math/3D/AABB3D.h"
#include "Scene/World/World.h"

namespace RNGOEngine::Systems::Core
{
    void CollisionSystem::Update(RNGOEngine::Core::World& world, EngineSystemContext& context)
    {
        EngineSystem::Update(world, context);
        CollisionList collisions = {};

        SphereToSphereCollisionDetection(world, collisions);
        BoxToBoxCollisionDetection(world, collisions);

        // Register collisions for both engine and game systems. This will copy the data, but it's fine for now.
        // TODO: Consider using a shared pointer if this becomes a problem.
        context.EngineResourceMapper->AddTransientResource(CollisionList{collisions}, true);
        context.GameResourceMapper->AddTransientResource(std::move(collisions), true);
    }

    void CollisionSystem::SphereToSphereCollisionDetection(
        RNGOEngine::Core::World& world, CollisionList& collisions
    )
    {
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
    }

    void CollisionSystem::BoxToBoxCollisionDetection(
        RNGOEngine::Core::World& world, CollisionList& collisions
    )
    {
        // Box -> Box CD
        const auto boxView = world.GetRegistry().view<Components::Transform, Components::BoxCollider>();

        // Simple O(n^2) for now
        for (const auto& [entityA, transformA, boxA] : boxView.each())
        {
            for (const auto& [entityB, transformB, boxB] : boxView.each())
            {
                if (entityA >= entityB)
                {
                    continue;
                }

                const glm::vec3 minA = transformA.Position - boxA.HalfExtents;
                const glm::vec3 maxA = transformA.Position + boxA.HalfExtents;
                const glm::vec3 minB = transformB.Position - boxB.HalfExtents;
                const glm::vec3 maxB = transformB.Position + boxB.HalfExtents;

                const Math::AABB3D<float> aabbA{{minA.x, minA.y, minA.z}, {maxA.x, maxA.y, maxA.z}};
                const Math::AABB3D<float> aabbB{{minB.x, minB.y, minB.z}, {maxB.x, maxB.y, maxB.z}};

                if (aabbA.Intersects(aabbB))
                {
                    collisions.collisions.emplace_back(entityA, entityB);
                }
            }
        }
    }
}