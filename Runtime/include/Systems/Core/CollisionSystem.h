//
// Created by ringo on 2026-01-27.
//

#pragma once

#include <entt/entt.hpp>

#include "Systems/ISystem.h"

namespace RNGOEngine::Systems::Core
{
    struct CollisionData
    {
        entt::entity entityA;
        entt::entity entityB;
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