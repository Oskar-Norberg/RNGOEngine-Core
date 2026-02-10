//
// Created by Oskar.Norberg on 2026-02-10.
//

#include "ECS/Systems/Core/Physics/RigidbodyCollisionResolutionSystem.h"

#include "ECS/Systems/Core/Physics/CollisionSystem.h"
#include "ECS/Systems/SystemContext.h"
#include "Scene/World/World.h"

namespace RNGOEngine::Systems::Core
{
    void RigidbodyCollisionResolutionSystem::Update(
        RNGOEngine::Core::World& world, EngineSystemContext& context
    )
    {
        EngineSystem::Update(world, context);

        if (!context.DoRunPhysics)
        {
            return;
        }

        CollisionList collisions;
        // TODO: What the fuck is this TryGet pattern? Using an out ref???
        bool found = context.EngineResourceMapper->TryGetTransientResource(collisions);

        if (!found)
        {
            return;
        }

        auto& registry = world.GetRegistry();
        for (const auto& [entityA, entityB, contactPoint, contactNormal, penetrationDepth] :
             collisions.collisions)
        {
            auto& transformA = registry.get<Components::Transform>(entityA);
            auto& transformB = registry.get<Components::Transform>(entityB);

            auto& rigidbodyA = registry.get<Components::Rigidbody>(entityA);
            auto& rigidbodyB = registry.get<Components::Rigidbody>(entityB);

            if (rigidbodyA.IsKinematic && rigidbodyB.IsKinematic)
            {
                continue;
            }

            if (rigidbodyA.IsKinematic)
            {
                KinematicToNonKinematicResolution(
                    transformA, transformB, contactPoint, contactNormal, penetrationDepth
                );
            }
            else if (rigidbodyB.IsKinematic)
            {
                KinematicToNonKinematicResolution(
                    transformB, transformA, contactPoint, contactNormal, penetrationDepth
                );
            }
            else
            {
                NonKinematicResolution(transformA, transformB, contactPoint, contactNormal, penetrationDepth);
            }
        }
    }

    void RigidbodyCollisionResolutionSystem::KinematicToNonKinematicResolution(
        Components::Transform& kinematicTransform, Components::Transform& nonKinematicTransform,
        const glm::vec3 contactPoint, const glm::vec3 contactNormal, const float penetrationDepth
    )
    {
        nonKinematicTransform.Position += contactNormal * penetrationDepth;

        // Velocity?
    }

    void RigidbodyCollisionResolutionSystem::NonKinematicResolution(
        Components::Transform& transformA, Components::Transform& transformB, glm::vec3 contactPoint,
        const glm::vec3 contactNormal, const float penetrationDepth
    )
    {
        transformA.Position += contactNormal * (penetrationDepth / 2.0f);
        transformB.Position -= contactNormal * (penetrationDepth / 2.0f);

        // Velocity?
    }
}