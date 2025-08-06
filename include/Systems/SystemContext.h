//
// Created by SkummisDenAndre on 2025-06-16.
//

#pragma once

#include "EventQueue/EventQueue.h"
#include "ResourceMapper.h"

struct SystemContext
{
    // Should be a time-span, but for now we use a float.
    float deltaTime = 0.0f;

    EventQueue eventQueue;
    ResourceMapper resourceMapper;
};
