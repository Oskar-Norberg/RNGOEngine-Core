//
// Created by ringo on 2025-11-12.
//

#pragma once

#include "Systems/ISystem.h"

namespace RNGOEngine::Editor
{
    class FreeFlyCameraSystem : public Systems::IGameSystem
    {
    public:
        void Update(Core::World& world, Systems::SystemContext& context) override;

    private:
        static constexpr auto CAMERA_SENSITIVITY = 100.0f;
        static constexpr auto CAMERA_SPEED = 10.0f;
        static constexpr auto CAMERA_BOOST_MULTIPLIER = 2.0f;
    };
}