//
// Created by ringo on 2026-01-27.
//

#pragma once

#include <entt/entt.hpp>

#include "ECS/Systems/ISystem.h"
#include "glm/vec3.hpp"

namespace RNGOEngine::Systems::Core
{
    struct CollisionData
    {
        entt::entity EntityA;
        entt::entity EntityB;

        glm::vec3 ContactPoint;
        glm::vec3 ContactNormal;
        float PenetrationDepth;
    };

    struct CollisionList
    {
        std::vector<CollisionData> collisions;
    };

    // TODO: Broadphase
    class CollisionSystem : public EngineSystem
    {
    public:
        void Update(RNGOEngine::Core::World& world, EngineSystemContext& context) override;

    private:
        void SphereToSphereCollisionDetection(RNGOEngine::Core::World& world, CollisionList& collisions);

        void BoxToBoxCollisionDetection(RNGOEngine::Core::World& world, CollisionList& collisions);

        void SphereToBoxCollisionDetection(RNGOEngine::Core::World& world, CollisionList& collisions);
    };
}