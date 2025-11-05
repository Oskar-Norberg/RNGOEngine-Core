//
// Created by SkummisDenAndre on 2025-06-16.
//

#pragma once

#include "World/World.h"

namespace RNGOEngine::Core
{
    class Engine;
}

namespace RNGOEngine::Core
{
    // TODO: This should not be a templated class. This should load from a serialized file. This is however a large task.
    class Scene
    {
    public:
        virtual ~Scene() = default;

        virtual void Initialize(Engine& engine) {}
        virtual void Exit(Engine& engine) {}

        World world;
    };
}