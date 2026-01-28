//
// Created by Oskar.Norberg on 2026-01-28.
//

#pragma once

#pragma once

#include "Renderer/API/RenderAPI.h"

namespace RNGOEngine::Systems::Core
{
    class EndFrameSystem : public EngineSystem
    {
    public:
        void Update(RNGOEngine::Core::World& world, EngineSystemContext& context) override
        {
            auto& renderAPI = *context.Renderer;
            renderAPI.EndFrame();
        }
    };
}