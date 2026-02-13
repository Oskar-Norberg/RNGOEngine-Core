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
        SphereToBoxCollisionDetection(world, collisions);

        // Register collisions for both engine and game systems. This will copy the data, but it's fine for now.
        // TODO: Consider using a shared pointer if this becomes a problem.
        context.EngineResourceMapper->AddTransientResource(CollisionList{collisions}, true);
        context.GameResourceMapper->AddTransientResource(std::move(collisions), true);
    }

    void CollisionSystem::SphereToSphereCollisionDetection(
        RNGOEngine::Core::World& world, CollisionList& collisions
    )
    {
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
                    const auto contactPoint = (transformA.Position + transformB.Position) / 2.0f;
                    const auto contactNormal = glm::normalize(transformB.Position - transformA.Position);
                    const auto penetrationDepth = (sphereA.Radius + sphereB.Radius) -
                                                  glm::length(transformA.Position - transformB.Position);

                    CollisionData collisionData = {
                        .EntityA = entityA,
                        .EntityB = entityB,
                        .ContactPoint = contactPoint,
                        .ContactNormal = contactNormal,
                        .PenetrationDepth = penetrationDepth,
                    };
                    collisions.collisions.emplace_back(collisionData);
                }
            }
        }
    }

    void CollisionSystem::BoxToBoxCollisionDetection(
        RNGOEngine::Core::World& world, CollisionList& collisions
    )
    {
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
                    const auto contactPoint = (transformA.Position + transformB.Position) / 2.0f;
                    const auto contactNormal = glm::normalize(transformB.Position - transformA.Position);
                    const auto penetrationDepth = glm::length(glm::min(maxA, maxB) - glm::max(minA, minB));

                    CollisionData collisionData = {
                        .EntityA = entityA,
                        .EntityB = entityB,
                        .ContactPoint = contactPoint,
                        .ContactNormal = contactNormal,
                        .PenetrationDepth = penetrationDepth,
                    };
                    collisions.collisions.emplace_back(collisionData);
                }
            }
        }
    }

    void CollisionSystem::SphereToBoxCollisionDetection(
        RNGOEngine::Core::World& world, CollisionList& collisions
    )
    {
        const auto boxView = world.GetRegistry().view<Components::Transform, Components::BoxCollider>();
        const auto sphereView = world.GetRegistry().view<Components::Transform, Components::SphereCollider>();

        for (const auto& [boxEntity, boxTransform, boxCollider] : boxView.each())
        {
            for (const auto& [sphereEntity, sphereTransform, sphereCollider] : sphereView.each())
            {
                const glm::vec3 boxMin = boxTransform.Position - boxCollider.HalfExtents;
                const glm::vec3 boxMax = boxTransform.Position + boxCollider.HalfExtents;

                const glm::vec3 closestPoint = glm::clamp(sphereTransform.Position, boxMin, boxMax);

                const float distanceSquared = glm::dot(
                    closestPoint - sphereTransform.Position, closestPoint - sphereTransform.Position
                );

                if (distanceSquared <= sphereCollider.Radius * sphereCollider.Radius)
                {
                    const auto contactPoint = closestPoint;
                    const auto contactNormal = glm::normalize(sphereTransform.Position - closestPoint);
                    const auto penetrationDepth = sphereCollider.Radius - glm::sqrt(distanceSquared);

                    CollisionData collisionData = {
                        .EntityA = boxEntity,
                        .EntityB = sphereEntity,
                        .ContactPoint = contactPoint,
                        .ContactNormal = contactNormal,
                        .PenetrationDepth = penetrationDepth,
                    };
                    collisions.collisions.emplace_back(collisionData);
                }
            }
        }
    }
}