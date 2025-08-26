//
// Created by Oskar.Norberg on 2025-08-26.
//

#pragma once
#include <vector>

#include "RenderHandles.h"

namespace RNGOEngine::Core::Renderer
{

    struct Drawable
    {
        
        
        MeshHandle mesh;
        ShaderHandle shader;
    };

    struct DrawQueue
    {
        std::vector<Drawable> opaqueObjects;
    };
}