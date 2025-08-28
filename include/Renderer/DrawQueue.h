//
// Created by Oskar.Norberg on 2025-08-26.
//

#pragma once

#include <vector>

#include "RenderID.h"
#include "Components/Components.h"

namespace RNGOEngine::Core::Renderer
{
    struct Drawable
    {
        Components::Transform transform;
        MeshID mesh;
        MaterialID material;
    };

    struct DrawQueue
    {
        // Camera Properties
        Components::Camera camera;
        Components::Transform cameraTransform;

        // TODO: Lights
        // TODO: Skybox/SceneColor

        // Opaque Objects
        std::vector<Drawable> opaqueObjects;

        // TODO: Transparents
        
    };
}