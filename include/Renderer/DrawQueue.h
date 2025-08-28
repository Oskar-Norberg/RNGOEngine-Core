//
// Created by Oskar.Norberg on 2025-08-26.
//

#pragma once

#include <vector>

#include "RenderID.h"

namespace RNGOEngine::Core::Renderer
{

    struct Drawable
    {
        // TODO: Add transform
        glm::mat4 transform = glm::mat4(1.0f);
        MeshID mesh;
        MaterialID material;
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