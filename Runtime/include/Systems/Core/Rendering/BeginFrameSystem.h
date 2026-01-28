//
// Created by Oskar.Norberg on 2026-01-28.
//

#pragma once

#include "Renderer/API/RenderAPI.h"
#include "Systems/ISystem.h"

namespace RNGOEngine::Systems::Core
{
    class BeginFrameSystem : public EngineSystem
    {
    public:
        void Update(RNGOEngine::Core::World& world, EngineSystemContext& context) override
        {
            auto& renderAPI = *context.Renderer;
            renderAPI.BeginFrame();
        }
    };
}