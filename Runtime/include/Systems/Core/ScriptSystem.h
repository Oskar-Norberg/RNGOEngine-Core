//
// Created by Oskar.Norberg on 2026-01-22.
//

#pragma once

#include "Systems/ISystem.h"

namespace RNGOEngine::Systems::Core
{
    class ScriptSystem : public EngineSystem
    {
    public:
        void Update(RNGOEngine::Core::World& world, EngineSystemContext& context) override;
    };
}