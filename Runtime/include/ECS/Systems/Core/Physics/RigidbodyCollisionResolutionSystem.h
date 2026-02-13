//
// Created by Oskar.Norberg on 2026-02-10.
//

#pragma once

#include "ECS/Components/Components.h"
#include "ECS/Systems/ISystem.h"

namespace RNGOEngine::Systems::Core
{
    class RigidbodyCollisionResolutionSystem : public EngineSystem
    {
    public:
        void Update(RNGOEngine::Core::World& world, EngineSystemContext& context) override;

    private:
        void KinematicToNonKinematicResolution(
            Components::Transform& kinematicTransform, Components::Transform& nonKinematicTransform,
            glm::vec3 contactPoint, glm::vec3 contactNormal, float penetrationDepth
        );

        void NonKinematicResolution(
            Components::Transform& transformA, Components::Transform& transformB, glm::vec3 contactPoint,
            glm::vec3 contactNormal, float penetrationDepth
        );
    };
}