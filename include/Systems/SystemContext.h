//
// Created by SkummisDenAndre on 2025-06-16.
//

#pragma once

#include "ResourceMap.h"

struct SystemContext
{
    // Should be a time-span, but for now we use a float.
    float deltaTime = 0.0f;

    ResourceMap resourceMap;
};
