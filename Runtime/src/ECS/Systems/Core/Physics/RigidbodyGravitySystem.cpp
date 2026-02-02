//
// Created by Oskar.Norberg on 2026-01-29.
//

#include "ECS/Systems/Core/Physics/RigidbodyGravitySystem.h"

#include "ECS/Systems/SystemContext.h"
#include "Scene/World/World.h"

namespace RNGOEngine::Systems::Core
{

    void RigidbodyGravitySystem::Update(RNGOEngine::Core::World& world, EngineSystemContext& context)
    {
        EngineSystem::Update(world, context);

        if (!context.DoRunPhysics)
        {
            return;
        }

        const auto rigidbodyView = world.GetRegistry().view<Components::Transform, Components::Rigidbody>();

        for (const auto& [entity, transform, rigidbody] : rigidbodyView.each())
        {
            if (!rigidbody.HasGravity)
            {
                continue;
            }

            transform.Position.y -= world.GetGravity() * context.DeltaTime;
        }
    }
}