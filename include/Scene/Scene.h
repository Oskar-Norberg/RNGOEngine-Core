//
// Created by SkummisDenAndre on 2025-06-16.
//

#pragma once

#include "World/World.h"

namespace RNGOEngine::Core
{
    class Scene
    {
    public:
        virtual ~Scene() = default;

        virtual void Initialize() {}
        virtual void Update(float deltaTime) {}
        virtual void Exit() {}

        World world;
    };
}