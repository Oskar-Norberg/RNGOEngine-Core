//
// Created by ringo on 2025-11-08.
//

#pragma once

#include "RenderPassResources.h"
#include "Renderer/DrawQueue.h"

namespace RNGOEngine::Core::Renderer
{
    struct RenderContext
    {
        // TODO: I hate that the DrawQueue is owned here. This should be a ref/pointer?
        DrawQueue drawQueue;
        RenderPassResources renderPassResources;
    };
}