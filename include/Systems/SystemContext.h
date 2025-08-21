//
// Created by SkummisDenAndre on 2025-06-16.
//

#pragma once

#include "EventQueue/EventQueue.h"
#include "ResourceMapper.h"
#include "Utilities/JobSystem/JobSystem.h"

namespace RNGOEngine::Systems
{
    struct SystemContext
    {
        // Should be a time-span, but for now we use a float.
        float deltaTime = 0.0f;

        Events::EventQueue eventQueue;
        Resources::ResourceMapper resourceMapper;
        Utilities::JobSystem jobSystem;
    };
}