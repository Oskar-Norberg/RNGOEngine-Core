//
// Created by ringo on 2026-01-27.
//

#pragma once

#include <entt/entt.hpp>

#include "ECS/Systems/ISystem.h"

namespace RNGOEngine::Systems::Core
{
    struct CollisionData
    {
        entt::entity EntityA;
        entt::entity EntityB;
    };

    struct CollisionList
    {
        std::vector<CollisionData> collisions;
    };

    class CollisionSystem : public EngineSystem
    {
    public:
        void Update(RNGOEngine::Core::World& world, EngineSystemContext& context) override;
    };
}