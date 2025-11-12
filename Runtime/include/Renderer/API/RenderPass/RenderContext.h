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
        DrawQueue drawQueue;
        RenderPassResources renderPassResources;
    };
}