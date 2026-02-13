//
// Created by Oskar.Norberg on 2026-02-10.
//

#include "ECS/Systems/Core/Physics/RigidbodyApplyForces.h"

#include "ECS/Systems/SystemContext.h"
#include "Scene/World/World.h"

namespace RNGOEngine::Systems::Core
{
    void RigidbodyApplyForcesSystem::Update(RNGOEngine::Core::World& world, EngineSystemContext& context)
    {
        EngineSystem::Update(world, context);

        if (!context.DoRunPhysics)
        {
            return;
        }

        const auto rigidbodyView = world.GetRegistry().view<Components::Transform, Components::Rigidbody>();

        for (const auto& [entity, transform, rigidbody] : rigidbodyView.each())
        {
            if (rigidbody.IsKinematic)
            {
                continue;
            }

            transform.Position += rigidbody.LinearVelocity * context.DeltaTime;
        }
    }
}