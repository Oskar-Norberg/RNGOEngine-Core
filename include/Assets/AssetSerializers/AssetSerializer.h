//
// Created by Oskar.Norberg on 2025-11-03.
//

#pragma once

#include <yaml-cpp/emitter.h>

#include "Assets/Asset.h"

namespace RNGOEngine::AssetHandling
{
    class AssetSerializer
    {
    public:
        virtual ~AssetSerializer() = default;

        virtual void Serialize(const AssetMetadata& metadata, YAML::Emitter& emitter);
    };
}