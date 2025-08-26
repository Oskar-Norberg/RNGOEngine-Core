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
        // TODO: Add transform
        
        MeshHandle mesh;
        ShaderHandle shader;
    };

    struct DrawQueue
    {
        // TODO: Camera
        // TODO: Lights
        // TODO: Skybox/SceneColor
        // TODO: Transparents
        
        std::vector<Drawable> opaqueObjects;
    };
}