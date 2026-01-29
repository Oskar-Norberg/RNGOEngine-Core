//
// Created by SkummisDenAndre on 2025-06-16.
//

#pragma once

#include <yaml-cpp/yaml.h>

#include "World/World.h"

namespace RNGOEngine::Core
{
    class Scene
    {
    public:
        void Deserialize(YAML::Node& node);
        void Serialize(YAML::Emitter& emitter) const;

    public:
        Scene Copy() const;

        World world;
    };
}