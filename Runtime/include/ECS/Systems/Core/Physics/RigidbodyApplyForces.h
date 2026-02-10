//
// Created by Oskar.Norberg on 2026-02-10.
//

#pragma once

#include "ECS/Systems/ISystem.h"

namespace RNGOEngine::Systems::Core
{
    class RigidbodyApplyForcesSystem : public EngineSystem
    {
    public:
        void Update(RNGOEngine::Core::World& world, EngineSystemContext& context) override;
    };
}